// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

#pragma once

#include <stdint.h>

#include <algorithm>
#include <chrono>
#include <exception>
#include <functional>
#include <memory>
#include <optional>
#include <type_traits>
#include <utility>
#include <variant>
#include <vector>

#include "wpi/util/condition_variable.hpp"
#include "wpi/util/mutex.hpp"

namespace wpi::util {

template <typename T>
class PromiseFactory;

template <typename T>
class future;

template <typename T>
class promise;
template <>
class promise<void>;

namespace detail {

enum class FutureStatus { PENDING, VALUE, ERROR, CANCELLED };

class PromiseFactoryStateBase {
 public:
  bool IsActive() const noexcept;
  void Notify() noexcept;

 protected:
  mutable wpi::util::mutex m_resultMutex;
  wpi::util::condition_variable m_resultCv;
  bool m_closed = false;
  uint64_t m_uid = 0;
};

template <typename T>
struct FutureResult {
  using Value = std::conditional_t<std::is_void_v<T>, std::monostate, T>;

  FutureResult() = default;
  explicit FutureResult(FutureStatus status_) : status{status_} {}
  FutureResult(FutureResult&& other) noexcept { *this = std::move(other); }
  FutureResult& operator=(FutureResult&& other) noexcept {
    status = other.status;
    error = std::move(other.error);
    value.reset();
    if (other.value) {
      // A throwing value move must settle the request, including when the
      // request vector grows or erases an entry.
      try {
        value.emplace(std::move(*other.value));
      } catch (...) {
        status = FutureStatus::ERROR;
        error = std::current_exception();
      }
    }
    return *this;
  }

  bool IsReady() const {
    return status == FutureStatus::VALUE || status == FutureStatus::ERROR;
  }

  FutureStatus status = FutureStatus::PENDING;
  std::optional<Value> value;
  std::exception_ptr error;
};

// One shared allocation per factory; requests and continuations stay in
// vectors. Handles retain this state, while continuations refer weakly to their
// output state so that chains do not create ownership cycles.
template <typename T>
class PromiseFactoryState final : public PromiseFactoryStateBase {
 public:
  using Result = FutureResult<T>;
  using ThenFunction = std::function<void(Result&)>;

  uint64_t CreateRequest() {
    std::scoped_lock lock(m_resultMutex);
    if (m_closed) {
      return 0;
    }
    uint64_t request = ++m_uid;
    m_requests.emplace_back(request);
    return request;
  }

  void Close() noexcept {
    std::vector<Then> thens;
    {
      std::scoped_lock lock(m_resultMutex);
      if (m_closed) {
        return;
      }
      m_closed = true;
      for (auto& request : m_requests) {
        if (request.result.status == FutureStatus::PENDING) {
          request.result.status = FutureStatus::CANCELLED;
        }
      }
      thens.swap(m_thens);
    }
    Notify();
    // Propagate cancellation and destroy continuation captures outside the
    // mutex; either can access another factory or reenter this one.
    for (auto& then : thens) {
      Result result{FutureStatus::CANCELLED};
      then.func(result);
    }
  }

  void IgnoreResult(uint64_t request) {
    Cancel(request);
    Result ignored;
    std::scoped_lock lock(m_resultMutex);
    auto it = FindRequest(request);
    if (it != m_requests.end()) {
      ignored = std::move(it->result);
      m_requests.erase(it);
    }
  }

  template <typename U>
  void SetValue(uint64_t request, U&& value) {
    std::unique_lock lock(m_resultMutex);
    auto it = FindRequest(request);
    if (it == m_requests.end() || it->result.status != FutureStatus::PENDING) {
      return;
    }
    Result result;
    try {
      result.value.emplace(std::forward<U>(value));
      result.status = FutureStatus::VALUE;
    } catch (...) {
      result.status = FutureStatus::ERROR;
      result.error = std::current_exception();
    }
    Finish(lock, it, result);
  }

  void SetDefaultValue(uint64_t request) noexcept {
    std::unique_lock lock(m_resultMutex);
    auto it = FindRequest(request);
    if (it == m_requests.end() || it->result.status != FutureStatus::PENDING) {
      return;
    }
    Result result;
    try {
      result.value.emplace();
      result.status = FutureStatus::VALUE;
    } catch (...) {
      result.status = FutureStatus::ERROR;
      result.error = std::current_exception();
    }
    Finish(lock, it, result);
  }

  void SetException(uint64_t request, std::exception_ptr error) {
    Result result{FutureStatus::ERROR};
    result.error = std::move(error);
    Complete(request, result);
  }

  void Cancel(uint64_t request) noexcept {
    Result result{FutureStatus::CANCELLED};
    Complete(request, result);
  }

  void SetThen(uint64_t request, ThenFunction func) {
    Result result{FutureStatus::CANCELLED};
    {
      std::scoped_lock lock(m_resultMutex);
      auto it = FindRequest(request);
      if (it != m_requests.end()) {
        if (it->result.status == FutureStatus::PENDING) {
          m_thens.emplace_back(request, std::move(func));
          return;
        }
        result = std::move(it->result);
        m_requests.erase(it);
      }
    }
    func(result);
  }

  bool IsReady(uint64_t request) noexcept {
    std::scoped_lock lock(m_resultMutex);
    auto it = FindRequest(request);
    return it != m_requests.end() && it->result.IsReady();
  }

  T GetResult(uint64_t request) {
    Result result{FutureStatus::CANCELLED};
    {
      std::unique_lock lock(m_resultMutex);
      m_resultCv.wait(lock, [&] { return IsFinished(request); });
      auto it = FindRequest(request);
      if (it != m_requests.end()) {
        result = std::move(it->result);
        m_requests.erase(it);
      }
    }
    if (result.status == FutureStatus::ERROR) {
      std::rethrow_exception(result.error);
    }
    if constexpr (!std::is_void_v<T>) {
      return result.value ? std::move(*result.value) : T{};
    }
  }

  void WaitResult(uint64_t request) {
    std::unique_lock lock(m_resultMutex);
    m_resultCv.wait(lock, [&] { return IsFinished(request); });
  }

  template <class Clock, class Duration>
  bool WaitResultUntil(
      uint64_t request,
      const std::chrono::time_point<Clock, Duration>& timeout_time) {
    std::unique_lock lock(m_resultMutex);
    m_resultCv.wait_until(lock, timeout_time,
                          [&] { return IsFinished(request); });
    auto it = FindRequest(request);
    return it != m_requests.end() && it->result.IsReady();
  }

 private:
  struct Request {
    explicit Request(uint64_t id_) : id{id_} {}
    uint64_t id;
    Result result;
  };

  struct Then {
    Then(uint64_t request_, ThenFunction func_)
        : request{request_}, func{std::move(func_)} {}
    uint64_t request;
    ThenFunction func;
  };

  // All request lookups require the mutex.  Look up by ID again after waiting;
  // another request can reallocate or erase entries in the vector.
  auto FindRequest(uint64_t request) {
    return std::find_if(m_requests.begin(), m_requests.end(),
                        [=](const auto& entry) { return entry.id == request; });
  }

  bool IsFinished(uint64_t request) {
    auto it = FindRequest(request);
    return it == m_requests.end() || it->result.status != FutureStatus::PENDING;
  }

  void Complete(uint64_t request, Result& result) {
    std::unique_lock lock(m_resultMutex);
    auto it = FindRequest(request);
    if (it != m_requests.end() && it->result.status == FutureStatus::PENDING) {
      Finish(lock, it, result);
    }
  }

  void Finish(std::unique_lock<wpi::util::mutex>& lock,
              typename std::vector<Request>::iterator request, Result& result) {
    ThenFunction func;
    auto then = std::find_if(
        m_thens.begin(), m_thens.end(),
        [&](const auto& entry) { return entry.request == request->id; });
    if (then != m_thens.end()) {
      func = std::move(then->func);
      m_thens.erase(then);
      m_requests.erase(request);
    } else {
      request->result = std::move(result);
    }
    lock.unlock();
    Notify();
    if (func) {
      func(result);
    }
  }

  std::vector<Request> m_requests;
  std::vector<Then> m_thens;
};

template <typename To, typename From>
struct FutureThen {
  template <typename F>
  static future<To> Create(
      const std::shared_ptr<PromiseFactoryState<From>>& from, uint64_t request,
      PromiseFactory<To>& factory, F&& func);
};

}  // namespace detail

/**
 * A promise factory for lightweight futures.
 *
 * Futures and promises share one state allocation per factory and may outlive
 * the factory. Closing or destroying the factory cancels pending requests;
 * completed results remain available. Request storage can allocate as it grows.
 * Calls on the factory itself still require the factory to be alive.
 *
 * Use CreateRequest() to create the future request id, and then CreateFuture()
 * and CreatePromise() to create future and promise objects.  A promise should
 * only be created once for any given request id, as should a future.
 *
 * @tparam T the "return" type of the promise/future
 */
template <typename T>
class PromiseFactory final {
  friend class future<T>;
  friend class promise<T>;
  template <typename To, typename From>
  friend struct detail::FutureThen;

 public:
  /** Constructs a factory with shared state for its requests. */
  PromiseFactory() = default;

  /**
   * Cancels pending requests, retaining completed results for their futures.
   */
  ~PromiseFactory() { Close(); }

  PromiseFactory(const PromiseFactory&) = delete;
  PromiseFactory& operator=(const PromiseFactory&) = delete;
  PromiseFactory(PromiseFactory&&) = delete;
  PromiseFactory& operator=(PromiseFactory&&) = delete;

  /**
   * Cancels pending requests and prevents new requests. Safe to call
   * repeatedly. Cancelled futures return a default value from get() and false
   * from timed waits. Their continuations are cancelled without calling user
   * functions. Continuations already selected for execution may run after
   * this call returns.
   */
  void Close() noexcept {
    auto state = m_state;
    state->Close();
  }

  /** @return True if the factory is accepting requests. */
  bool IsActive() const noexcept { return m_state->IsActive(); }

  /** Wakes threads waiting for results. Does not change request state. */
  void Notify() noexcept {
    auto state = m_state;
    state->Notify();
  }

  /** @return A new request ID, or zero if the factory is closed. */
  uint64_t CreateRequest() { return m_state->CreateRequest(); }

  /**
   * Discards a pending request or its unconsumed result.
   * @param request the request ID
   */
  void IgnoreResult(uint64_t request) {
    auto state = m_state;
    state->IgnoreResult(request);
  }

  /**
   * Creates a future.
   *
   * @param request the request id returned by CreateRequest()
   * @return the future
   */
  future<T> CreateFuture(uint64_t request);

  /**
   * Creates a future and makes it immediately ready.
   *
   * @param value the result
   * @return the future, or an invalid future if closed
   */
  future<T> MakeReadyFuture(T&& value);

  /**
   * Creates a promise.
   *
   * @param request the request id returned by CreateRequest()
   * @return the promise
   */
  promise<T> CreatePromise(uint64_t request);

  /**
   * Sets a value directly for a future without creating a promise object.
   * Identical to `CreatePromise(request).set_value(value)`.
   *
   * @param request request id, as returned by CreateRequest()
   * @param value lvalue
   */
  void SetValue(uint64_t request, const T& value);

  /**
   * Sets a value directly for a future without creating a promise object.
   * Identical to `CreatePromise(request).set_value(value)`.
   *
   * @param request request id, as returned by CreateRequest()
   * @param value rvalue
   */
  void SetValue(uint64_t request, T&& value);

  /**
   * Checks whether a request has a result or an exception.
   * @param request the request ID
   * @return False for pending, cancelled, or consumed requests
   */
  bool IsReady(uint64_t request) noexcept { return m_state->IsReady(request); }

  /**
   * Waits for and consumes a result, rethrowing any stored exception.
   * @param request the request ID
   * @return The result, or a default value if cancelled
   */
  T GetResult(uint64_t request) {
    auto state = m_state;
    return state->GetResult(request);
  }

  /**
   * Waits until a request completes or is cancelled.
   * @param request the request ID
   */
  void WaitResult(uint64_t request) {
    auto state = m_state;
    state->WaitResult(request);
  }

  /**
   * Waits for a request to complete, be cancelled, or reach the deadline.
   * @param request the request ID
   * @param timeout_time the deadline
   * @return True for a result or exception, false for cancellation or timeout
   */
  template <class Clock, class Duration>
  bool WaitResultUntil(
      uint64_t request,
      const std::chrono::time_point<Clock, Duration>& timeout_time) {
    auto state = m_state;
    return state->WaitResultUntil(request, timeout_time);
  }

  /** @return The default factory for this result type. */
  static PromiseFactory& GetInstance();

 private:
  std::shared_ptr<detail::PromiseFactoryState<T>> m_state =
      std::make_shared<detail::PromiseFactoryState<T>>();
};

/**
 * Explicit specialization for PromiseFactory<void>.
 * Shares the same lifetime and cancellation behavior as PromiseFactory<T>.
 */
template <>
class PromiseFactory<void> final {
  friend class future<void>;
  friend class promise<void>;
  template <typename To, typename From>
  friend struct detail::FutureThen;

 public:
  /** Constructs a factory with shared state for its requests. */
  PromiseFactory() = default;

  /**
   * Cancels pending requests, retaining completed results for their futures.
   */
  ~PromiseFactory() { Close(); }

  PromiseFactory(const PromiseFactory&) = delete;
  PromiseFactory& operator=(const PromiseFactory&) = delete;
  PromiseFactory(PromiseFactory&&) = delete;
  PromiseFactory& operator=(PromiseFactory&&) = delete;

  /**
   * Cancels pending requests and prevents new requests. Safe to call
   * repeatedly. Cancelled futures return a default value from get() and false
   * from timed waits. Their continuations are cancelled without calling user
   * functions. Continuations already selected for execution may run after
   * this call returns.
   */
  void Close() noexcept {
    auto state = m_state;
    state->Close();
  }

  /** @return True if the factory is accepting requests. */
  bool IsActive() const noexcept { return m_state->IsActive(); }

  /** Wakes threads waiting for results. Does not change request state. */
  void Notify() noexcept {
    auto state = m_state;
    state->Notify();
  }

  /** @return A new request ID, or zero if the factory is closed. */
  uint64_t CreateRequest() { return m_state->CreateRequest(); }

  /**
   * Discards a pending request or its unconsumed result.
   * @param request the request ID
   */
  void IgnoreResult(uint64_t request) {
    auto state = m_state;
    state->IgnoreResult(request);
  }

  /**
   * Creates a future.
   *
   * @param request the request id returned by CreateRequest()
   * @return the future
   */
  future<void> CreateFuture(uint64_t request);

  /**
   * Creates a future and makes it immediately ready.
   *
   * @return the future, or an invalid future if closed
   */
  future<void> MakeReadyFuture();

  /**
   * Creates a promise.
   *
   * @param request the request id returned by CreateRequest()
   * @return the promise
   */
  promise<void> CreatePromise(uint64_t request);

  /**
   * Sets a value directly for a future without creating a promise object.
   * Identical to `CreatePromise(request).set_value()`.
   *
   * @param request request id, as returned by CreateRequest()
   */
  void SetValue(uint64_t request);

  /**
   * Checks whether a request has a result or an exception.
   * @param request the request ID
   * @return False for pending, cancelled, or consumed requests
   */
  bool IsReady(uint64_t request) noexcept { return m_state->IsReady(request); }

  /**
   * Waits for and consumes a result, rethrowing any stored exception.
   * @param request the request ID
   */
  void GetResult(uint64_t request) {
    auto state = m_state;
    state->GetResult(request);
  }

  /**
   * Waits until a request completes or is cancelled.
   * @param request the request ID
   */
  void WaitResult(uint64_t request) {
    auto state = m_state;
    state->WaitResult(request);
  }

  /**
   * Waits for a request to complete, be cancelled, or reach the deadline.
   * @param request the request ID
   * @param timeout_time the deadline
   * @return True for a result or exception, false for cancellation or timeout
   */
  template <class Clock, class Duration>
  bool WaitResultUntil(
      uint64_t request,
      const std::chrono::time_point<Clock, Duration>& timeout_time) {
    auto state = m_state;
    return state->WaitResultUntil(request, timeout_time);
  }

  /** @return The default factory for this result type. */
  static PromiseFactory& GetInstance();

 private:
  std::shared_ptr<detail::PromiseFactoryState<void>> m_state =
      std::make_shared<detail::PromiseFactoryState<void>>();
};

/**
 * A lightweight version of std::future.
 *
 * Use either promise::get_future() or PromiseFactory::CreateFuture() to create.
 *
 * @tparam T the "return" type
 */
template <typename T>
class future final {
  friend class PromiseFactory<T>;
  friend class promise<T>;

 public:
  /**
   * Constructs an empty (invalid) future.
   */
  future() noexcept = default;

  /**
   * Moves a future, leaving the source invalid.
   * @param oth the source future
   */
  future(future&& oth) noexcept {
    this->m_request = oth.m_request;
    this->m_state = std::move(oth.m_state);
    oth.m_request = 0;
    oth.m_state = nullptr;
  }
  future(const future&) = delete;

  /**
   * Consumes a future and converts its result using a continuation.
   * @param oth the source future
   */
  template <typename R>
  future(future<R>&& oth)  // NOLINT
      : future(oth.then([](R&& val) -> T { return val; })) {}

  /**
   * Ignores the result of the future if it has not been retrieved.
   */
  ~future() {
    if (m_state) {
      m_state->IgnoreResult(m_request);
    }
  }

  /**
   * Discards this future's result and moves another future into it.
   * @param oth the source future
   * @return This future
   */
  future& operator=(future&& oth) noexcept {
    if (this != &oth) {
      future old{std::move(*this)};
      m_request = std::exchange(oth.m_request, 0);
      m_state = std::move(oth.m_state);
    }
    return *this;
  }

  future& operator=(const future&) = delete;

  /**
   * Gets the value.  Calls wait() if the value is not yet available.
   * Can only be called once.  The future will be marked invalid after the call.
   *
   * Rethrows any stored exception. Returns a default value if cancelled.
   *
   * @return The value provided by the corresponding promise.set_value().
   */
  T get() {
    if (m_state) {
      auto state = std::move(m_state);
      return state->GetResult(m_request);
    } else {
      return T();
    }
  }

  /**
   * Consumes this future and runs a function when its result is available.
   * Cancellation propagates without calling the function. Exceptions from the
   * function are stored in the returned future and rethrown by get().
   * @param factory factory for the returned future
   * @param func continuation function
   * @return A future for the continuation's result
   */
  template <typename R, typename F>
  future<R> then(PromiseFactory<R>& factory, F&& func) {
    if (!m_state) {
      return future<R>();
    }
    future input{std::move(*this)};
    auto out = detail::FutureThen<R, T>::Create(input.m_state, input.m_request,
                                                factory, std::forward<F>(func));
    input.m_state.reset();
    return out;
  }

  /**
   * Chains a continuation using the default factory for its result type.
   * @param func continuation function
   * @return A future for the continuation's result
   */
  template <typename F, typename R = typename std::invoke_result_t<F&&, T&&>>
  future<R> then(F&& func) {
    return then(PromiseFactory<R>::GetInstance(), std::forward<F>(func));
  }

  /** @return True for a value or exception, false if pending or cancelled. */
  bool is_ready() const noexcept {
    return m_state && m_state->IsReady(m_request);
  }

  /**
   * Checks if the future is valid.
   * A default-constructed future or one where get() has been called is invalid.
   *
   * @return True if valid
   */
  bool valid() const noexcept { return static_cast<bool>(m_state); }

  /**
   * Waits for the promise to provide a value.
   * Does not return until the value is available or the promise is destroyed
   * (in which case a default-constructed value is "returned").
   * Also returns if the factory closes and cancels the request.
   * If the value has already been provided, returns immediately.
   */
  void wait() const {
    if (m_state) {
      m_state->WaitResult(m_request);
    }
  }

  /**
   * Waits for the promise to provide a value, or the specified time has been
   * reached.
   *
   * @return True for a value or exception, false if cancelled or timed out.
   */
  template <class Clock, class Duration>
  bool wait_until(
      const std::chrono::time_point<Clock, Duration>& timeout_time) const {
    return m_state && m_state->WaitResultUntil(m_request, timeout_time);
  }

  /**
   * Waits for the promise to provide a value, or the specified amount of time
   * has elapsed.
   *
   * @return True for a value or exception, false if cancelled or timed out.
   */
  template <class Rep, class Period>
  bool wait_for(
      const std::chrono::duration<Rep, Period>& timeout_duration) const {
    return wait_until(std::chrono::steady_clock::now() + timeout_duration);
  }

 private:
  future(std::shared_ptr<detail::PromiseFactoryState<T>> state,
         uint64_t request) noexcept
      : m_request(request), m_state(request ? std::move(state) : nullptr) {}

  uint64_t m_request = 0;
  std::shared_ptr<detail::PromiseFactoryState<T>> m_state;
};

/**
 * Explicit specialization for future<void>.
 */
template <>
class future<void> final {
  friend class PromiseFactory<void>;
  friend class promise<void>;

 public:
  /**
   * Constructs an empty (invalid) future.
   */
  future() noexcept = default;

  /**
   * Moves a future, leaving the source invalid.
   * @param oth the source future
   */
  future(future&& oth) noexcept {
    m_request = oth.m_request;
    m_state = std::move(oth.m_state);
    oth.m_request = 0;
    oth.m_state = nullptr;
  }
  future(const future&) = delete;

  /**
   * Ignores the result of the future if it has not been retrieved.
   */
  ~future() {
    if (m_state) {
      m_state->IgnoreResult(m_request);
    }
  }

  /**
   * Discards this future's result and moves another future into it.
   * @param oth the source future
   * @return This future
   */
  future& operator=(future&& oth) noexcept {
    if (this != &oth) {
      future old{std::move(*this)};
      m_request = std::exchange(oth.m_request, 0);
      m_state = std::move(oth.m_state);
    }
    return *this;
  }

  future& operator=(const future&) = delete;

  /**
   * Gets the value.  Calls wait() if the value is not yet available.
   * Can only be called once.  The future will be marked invalid after the call.
   * Rethrows any stored exception. Returns normally if cancelled.
   */
  void get() {
    if (m_state) {
      auto state = std::move(m_state);
      state->GetResult(m_request);
    }
  }

  /**
   * Consumes this future and runs a function when its result is available.
   * Cancellation propagates without calling the function. Exceptions from the
   * function are stored in the returned future and rethrown by get().
   * @param factory factory for the returned future
   * @param func continuation function
   * @return A future for the continuation's result
   */
  template <typename R, typename F>
  future<R> then(PromiseFactory<R>& factory, F&& func) {
    if (!m_state) {
      return future<R>();
    }
    future input{std::move(*this)};
    auto out = detail::FutureThen<R, void>::Create(
        input.m_state, input.m_request, factory, std::forward<F>(func));
    input.m_state.reset();
    return out;
  }

  /**
   * Chains a continuation using the default factory for its result type.
   * @param func continuation function
   * @return A future for the continuation's result
   */
  template <typename F, typename R = typename std::invoke_result_t<F&&>>
  future<R> then(F&& func) {
    return then(PromiseFactory<R>::GetInstance(), std::forward<F>(func));
  }

  /** @return True for a value or exception, false if pending or cancelled. */
  bool is_ready() const noexcept {
    return m_state && m_state->IsReady(m_request);
  }

  /**
   * Checks if the future is valid.
   * A default-constructed future or one where get() has been called is invalid.
   *
   * @return True if valid
   */
  bool valid() const noexcept { return static_cast<bool>(m_state); }

  /**
   * Waits for the promise to provide a value.
   * Does not return until the value is available or the promise is destroyed.
   * Also returns if the factory closes and cancels the request.
   * If the value has already been provided, returns immediately.
   */
  void wait() const {
    if (m_state) {
      m_state->WaitResult(m_request);
    }
  }

  /**
   * Waits for the promise to provide a value, or the specified time has been
   * reached.
   *
   * @return True for a value or exception, false if cancelled or timed out.
   */
  template <class Clock, class Duration>
  bool wait_until(
      const std::chrono::time_point<Clock, Duration>& timeout_time) const {
    return m_state && m_state->WaitResultUntil(m_request, timeout_time);
  }

  /**
   * Waits for the promise to provide a value, or the specified amount of time
   * has elapsed.
   *
   * @return True for a value or exception, false if cancelled or timed out.
   */
  template <class Rep, class Period>
  bool wait_for(
      const std::chrono::duration<Rep, Period>& timeout_duration) const {
    return wait_until(std::chrono::steady_clock::now() + timeout_duration);
  }

 private:
  future(std::shared_ptr<detail::PromiseFactoryState<void>> state,
         uint64_t request) noexcept
      : m_request(request), m_state(request ? std::move(state) : nullptr) {}

  uint64_t m_request = 0;
  std::shared_ptr<detail::PromiseFactoryState<void>> m_state;
};

/**
 * A lightweight version of std::promise.
 *
 * Use PromiseFactory::CreatePromise() to create.
 *
 * @tparam T the "return" type
 */
template <typename T>
class promise final {
  friend class PromiseFactory<T>;

 public:
  /**
   * Constructs a pending promise using the default factory.
   */
  promise() : m_state(PromiseFactory<T>::GetInstance().m_state) {
    m_request = m_state->CreateRequest();
  }

  /**
   * Moves a promise, leaving the source empty.
   * @param oth the source promise
   */
  promise(promise&& oth) noexcept
      : m_request(oth.m_request), m_state(std::move(oth.m_state)) {
    oth.m_request = 0;
    oth.m_state = nullptr;
  }

  promise(const promise&) = delete;

  /**
   * Sets the promised value to a default-constructed T if not already set.
   */
  ~promise() {
    if (auto state = std::move(m_state)) {
      state->SetDefaultValue(m_request);
    }
  }

  /**
   * Supplies a default value for this promise and moves another promise into
   * it.
   * @param oth the source promise
   * @return This promise
   */
  promise& operator=(promise&& oth) noexcept {
    if (this != &oth) {
      promise old{std::move(*this)};
      m_request = std::exchange(oth.m_request, 0);
      m_state = std::move(oth.m_state);
    }
    return *this;
  }

  promise& operator=(const promise&) = delete;

  /**
   * Swaps this promise with another one.
   * @param oth the other promise
   */
  void swap(promise& oth) noexcept {
    std::swap(m_request, oth.m_request);
    std::swap(m_state, oth.m_state);
  }

  /**
   * Gets a future for this promise. Must only be called once per request.
   *
   * @return The future
   */
  future<T> get_future() noexcept { return future<T>(m_state, m_request); }

  /**
   * Sets the promised value.
   * Only effective once (subsequent calls will be ignored).
   *
   * @param value The value to provide to the waiting future
   */
  void set_value(const T& value) {
    if (m_state) {
      auto state = std::move(m_state);
      state->SetValue(m_request, value);
    }
  }

  /**
   * Sets the promised value.
   * Only effective once (subsequent calls will be ignored).
   *
   * @param value The value to provide to the waiting future
   */
  void set_value(T&& value) {
    if (m_state) {
      auto state = std::move(m_state);
      state->SetValue(m_request, std::move(value));
    }
  }

 private:
  promise(std::shared_ptr<detail::PromiseFactoryState<T>> state,
          uint64_t request) noexcept
      : m_request(request), m_state(request ? std::move(state) : nullptr) {}

  uint64_t m_request = 0;
  std::shared_ptr<detail::PromiseFactoryState<T>> m_state;
};

/**
 * Explicit specialization for promise<void>.
 */
template <>
class promise<void> final {
  friend class PromiseFactory<void>;

 public:
  /**
   * Constructs a pending promise using the default factory.
   */
  promise() : m_state(PromiseFactory<void>::GetInstance().m_state) {
    m_request = m_state->CreateRequest();
  }

  /**
   * Moves a promise, leaving the source empty.
   * @param oth the source promise
   */
  promise(promise&& oth) noexcept
      : m_request(oth.m_request), m_state(std::move(oth.m_state)) {
    oth.m_request = 0;
    oth.m_state = nullptr;
  }

  promise(const promise&) = delete;

  /**
   * Sets the promised value if not already set.
   */
  ~promise() {
    if (auto state = std::move(m_state)) {
      state->SetDefaultValue(m_request);
    }
  }

  /**
   * Supplies a default value for this promise and moves another promise into
   * it.
   * @param oth the source promise
   * @return This promise
   */
  promise& operator=(promise&& oth) noexcept {
    if (this != &oth) {
      promise old{std::move(*this)};
      m_request = std::exchange(oth.m_request, 0);
      m_state = std::move(oth.m_state);
    }
    return *this;
  }

  promise& operator=(const promise&) = delete;

  /**
   * Swaps this promise with another one.
   * @param oth the other promise
   */
  void swap(promise& oth) noexcept {
    std::swap(m_request, oth.m_request);
    std::swap(m_state, oth.m_state);
  }

  /**
   * Gets a future for this promise. Must only be called once per request.
   *
   * @return The future
   */
  future<void> get_future() noexcept {
    return future<void>(m_state, m_request);
  }

  /**
   * Sets the promised value.
   * Only effective once (subsequent calls will be ignored).
   */
  void set_value() {
    if (m_state) {
      auto state = std::move(m_state);
      state->SetDefaultValue(m_request);
    }
  }

 private:
  promise(std::shared_ptr<detail::PromiseFactoryState<void>> state,
          uint64_t request) noexcept
      : m_request(request), m_state(request ? std::move(state) : nullptr) {}

  uint64_t m_request = 0;
  std::shared_ptr<detail::PromiseFactoryState<void>> m_state;
};

/**
 * Constructs a valid future with the value set.
 * @param value the result
 * @return A ready future
 */
template <typename T>
inline future<T> make_ready_future(T&& value) {
  return PromiseFactory<T>::GetInstance().MakeReadyFuture(
      std::forward<T>(value));
}

/**
 * Constructs a valid future with the value set.
 * @return A ready future
 */
inline future<void> make_ready_future() {
  return PromiseFactory<void>::GetInstance().MakeReadyFuture();
}

template <typename T>
inline future<T> PromiseFactory<T>::CreateFuture(uint64_t request) {
  return future<T>{m_state, request};
}

inline future<void> PromiseFactory<void>::CreateFuture(uint64_t request) {
  return future<void>{m_state, request};
}

template <typename T>
future<T> PromiseFactory<T>::MakeReadyFuture(T&& value) {
  auto state = m_state;
  uint64_t request = state->CreateRequest();
  future<T> result{state, request};
  state->SetValue(request, std::move(value));
  return result;
}

inline future<void> PromiseFactory<void>::MakeReadyFuture() {
  auto state = m_state;
  uint64_t request = state->CreateRequest();
  future<void> result{state, request};
  state->SetDefaultValue(request);
  return result;
}

template <typename T>
inline promise<T> PromiseFactory<T>::CreatePromise(uint64_t request) {
  return promise<T>{m_state, request};
}

inline promise<void> PromiseFactory<void>::CreatePromise(uint64_t request) {
  return promise<void>{m_state, request};
}

template <typename T>
void PromiseFactory<T>::SetValue(uint64_t request, const T& value) {
  auto state = m_state;
  state->SetValue(request, value);
}

template <typename T>
void PromiseFactory<T>::SetValue(uint64_t request, T&& value) {
  auto state = m_state;
  state->SetValue(request, std::move(value));
}

inline void PromiseFactory<void>::SetValue(uint64_t request) {
  auto state = m_state;
  state->SetDefaultValue(request);
}

template <typename T>
PromiseFactory<T>& PromiseFactory<T>::GetInstance() {
  static PromiseFactory<T> inst;
  return inst;
}

namespace detail {

template <typename To, typename From>
template <typename F>
future<To> FutureThen<To, From>::Create(
    const std::shared_ptr<PromiseFactoryState<From>>& from, uint64_t request,
    PromiseFactory<To>& factory, F&& func) {
  uint64_t outRequest = factory.CreateRequest();
  auto out = factory.CreateFuture(outRequest);
  std::weak_ptr<PromiseFactoryState<To>> destination = factory.m_state;
  if (outRequest == 0) {
    from->IgnoreResult(request);
    return out;
  }
  from->SetThen(request,
                [destination, outRequest, func = std::forward<F>(func)](
                    FutureResult<From>& result) mutable {
                  auto state = destination.lock();
                  if (!state) {
                    return;
                  }
                  if (result.status == FutureStatus::CANCELLED) {
                    state->Cancel(outRequest);
                    return;
                  }
                  if (result.status == FutureStatus::ERROR) {
                    state->SetException(outRequest, result.error);
                    return;
                  }
                  // This claims the callback before releasing the destination
                  // mutex. Close may subsequently cancel its output, in which
                  // case SetValue is a no-op.
                  if (!state->IsActive()) {
                    return;
                  }
                  try {
                    auto invoke = [&]() -> To {
                      if constexpr (std::is_void_v<From>) {
                        return std::invoke(func);
                      } else {
                        return std::invoke(func, std::move(*result.value));
                      }
                    };
                    if constexpr (std::is_void_v<To>) {
                      invoke();
                      state->SetDefaultValue(outRequest);
                    } else {
                      state->SetValue(outRequest, invoke());
                    }
                  } catch (...) {
                    state->SetException(outRequest, std::current_exception());
                  }
                });
  return out;
}

}  // namespace detail

}  // namespace wpi::util
