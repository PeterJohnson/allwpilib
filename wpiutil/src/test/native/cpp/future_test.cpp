// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

#include "wpi/util/future.hpp"

#include <barrier>
#include <chrono>
#include <memory>
#include <optional>
#include <stdexcept>
#include <thread>
#include <type_traits>
#include <utility>
#include <vector>

#include <catch2/catch_template_test_macros.hpp>
#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>
#include <catch2/matchers/catch_matchers_range_equals.hpp>
#include <catch2/matchers/catch_matchers_vector.hpp>

namespace wpi::util {

TEST_CASE("FutureTest Then", "[wpiutil]") {
  promise<bool> inPromise;
  future<int> outFuture =
      inPromise.get_future().then([](bool v) { return v ? 5 : 6; });

  inPromise.set_value(true);
  REQUIRE(outFuture.get() == 5);
}

TEST_CASE("FutureTest ThenSame", "[wpiutil]") {
  promise<bool> inPromise;
  future<bool> outFuture =
      inPromise.get_future().then([](bool v) { return !v; });

  inPromise.set_value(true);
  REQUIRE(outFuture.get() == false);
}

TEST_CASE("FutureTest ThenFromVoid", "[wpiutil]") {
  promise<void> inPromise;
  future<int> outFuture = inPromise.get_future().then([] { return 5; });

  inPromise.set_value();
  REQUIRE(outFuture.get() == 5);
}

TEST_CASE("FutureTest ThenToVoid", "[wpiutil]") {
  promise<bool> inPromise;
  future<void> outFuture = inPromise.get_future().then([](bool v) {});

  inPromise.set_value(true);
  REQUIRE(outFuture.is_ready());
}

TEST_CASE("FutureTest ThenVoidVoid", "[wpiutil]") {
  promise<void> inPromise;
  future<void> outFuture = inPromise.get_future().then([] {});

  inPromise.set_value();
  REQUIRE(outFuture.is_ready());
}

TEST_CASE("FutureTest Implicit", "[wpiutil]") {
  promise<bool> inPromise;
  future<int> outFuture = inPromise.get_future();

  inPromise.set_value(true);
  REQUIRE(outFuture.get() == 1);
}

TEST_CASE("FutureTest MoveSame", "[wpiutil]") {
  promise<bool> inPromise;
  future<bool> outFuture1 = inPromise.get_future();
  future<bool> outFuture(std::move(outFuture1));

  inPromise.set_value(true);
  REQUIRE(outFuture.get() == true);
}

TEST_CASE("FutureTest MoveVoid", "[wpiutil]") {
  promise<void> inPromise;
  future<void> outFuture1 = inPromise.get_future();
  future<void> outFuture(std::move(outFuture1));

  inPromise.set_value();
  REQUIRE(outFuture.is_ready());
}

TEMPLATE_TEST_CASE("FutureTest FactoryLifetime", "[wpiutil]", int, void) {
  std::optional<promise<TestType>> producer;
  future<TestType> result;
  {
    PromiseFactory<TestType> factory;
    producer.emplace(factory.CreatePromise(factory.CreateRequest()));
    result = producer->get_future();
  }

  REQUIRE(result.valid());
  REQUIRE_FALSE(result.is_ready());
  REQUIRE_FALSE(result.wait_for(std::chrono::seconds{0}));
  result.wait();
  // A promise retained by asynchronous work must also survive its factory.
  SECTION("Late completion") {
    if constexpr (std::is_void_v<TestType>) {
      producer->set_value();
    } else {
      producer->set_value(42);
    }
  }
  SECTION("Abandon promise") {
    producer.reset();
  }
  if constexpr (std::is_void_v<TestType>) {
    result.get();
  } else {
    REQUIRE(result.get() == 0);
  }
  REQUIRE_FALSE(result.valid());
  producer.reset();
}

TEMPLATE_TEST_CASE("FutureTest Close", "[wpiutil]", int, void) {
  future<TestType> completed;
  future<TestType> pending;
  {
    PromiseFactory<TestType> factory;
    pending = factory.CreateFuture(factory.CreateRequest());
    if constexpr (std::is_void_v<TestType>) {
      completed = factory.MakeReadyFuture();
    } else {
      completed = factory.MakeReadyFuture(42);
    }

    factory.Close();
    factory.Close();
    REQUIRE_FALSE(factory.IsActive());
    REQUIRE(factory.CreateRequest() == 0);
    REQUIRE_FALSE(pending.wait_for(std::chrono::seconds{0}));
    if constexpr (std::is_void_v<TestType>) {
      REQUIRE_FALSE(factory.MakeReadyFuture().valid());
    } else {
      REQUIRE_FALSE(factory.MakeReadyFuture(1).valid());
    }
  }

  REQUIRE(completed.is_ready());
  REQUIRE(completed.wait_for(std::chrono::seconds{0}));
  if constexpr (std::is_void_v<TestType>) {
    completed.get();
    pending.get();
  } else {
    REQUIRE(completed.get() == 42);
    REQUIRE(pending.get() == 0);
  }
  REQUIRE_FALSE(completed.valid());
  REQUIRE_FALSE(pending.valid());
}

TEMPLATE_TEST_CASE("FutureTest PromiseMoveAssignment", "[wpiutil]", int, void) {
  promise<TestType> producer;
  auto abandoned = producer.get_future();
  producer = promise<TestType>{};
  REQUIRE(abandoned.is_ready());
  if constexpr (std::is_void_v<TestType>) {
    abandoned.get();
  } else {
    REQUIRE(abandoned.get() == 0);
  }

  auto result = producer.get_future();
  auto& same = producer;
  producer = std::move(same);
  if constexpr (std::is_void_v<TestType>) {
    producer.set_value();
    REQUIRE(result.is_ready());
    result.get();
  } else {
    producer.set_value(42);
    REQUIRE(result.get() == 42);
  }
}

TEMPLATE_TEST_CASE("FutureTest FutureMoveAssignment", "[wpiutil]", int, void) {
  PromiseFactory<TestType> factory;
  auto discarded = factory.CreateRequest();
  auto replacement = factory.CreateRequest();
  auto result = factory.CreateFuture(discarded);
  auto other = factory.CreateFuture(replacement);
  if constexpr (std::is_void_v<TestType>) {
    factory.SetValue(discarded);
    factory.SetValue(replacement);
  } else {
    factory.SetValue(discarded, 7);
    factory.SetValue(replacement, 42);
  }

  result = std::move(other);
  REQUIRE_FALSE(factory.IsReady(discarded));
  REQUIRE_FALSE(other.valid());
  auto& same = result;
  result = std::move(same);
  REQUIRE(result.is_ready());
  if constexpr (std::is_void_v<TestType>) {
    result.get();
  } else {
    REQUIRE(result.get() == 42);
  }
  REQUIRE_FALSE(result.valid());
}

TEST_CASE("FutureTest DiscardCompletedResult", "[wpiutil]") {
  PromiseFactory<std::shared_ptr<int>> factory;
  auto value = std::make_shared<int>(42);
  std::weak_ptr<int> lifetime = value;
  auto result = factory.MakeReadyFuture(std::move(value));

  SECTION("Destruction") {
    {
      auto discarded = std::move(result);
    }
  }
  SECTION("Move assignment") {
    auto replacement = factory.MakeReadyFuture(std::make_shared<int>(7));
    result = std::move(replacement);
    REQUIRE_FALSE(replacement.valid());
    auto& same = result;
    result = std::move(same);
    REQUIRE(*result.get() == 7);
  }
  REQUIRE(lifetime.expired());
}

TEST_CASE("FutureTest ThenSourceClosed", "[wpiutil]") {
  PromiseFactory<void> intermediate;
  PromiseFactory<int> destination;
  future<int> result;
  bool called = false;
  {
    PromiseFactory<int> source;
    result = source.CreateFuture(source.CreateRequest())
                 .then(intermediate, [&](int) { called = true; })
                 .then(destination, [&] {
                   called = true;
                   return 42;
                 });
  }
  REQUIRE_FALSE(result.wait_for(std::chrono::seconds{0}));
  REQUIRE(result.get() == 0);
  REQUIRE_FALSE(called);
  // Cancellation is per request; the other factories still accept work.
  REQUIRE(destination.MakeReadyFuture(7).get() == 7);
  REQUIRE(intermediate.MakeReadyFuture().is_ready());
}

TEST_CASE("FutureTest ThenDestinationDestroyed", "[wpiutil]") {
  promise<int> producer;
  future<int> result;
  bool called = false;
  {
    PromiseFactory<int> destination;
    result = producer.get_future().then(destination, [&](int value) {
      called = true;
      return value;
    });
    SECTION("Outstanding future") {}
    SECTION("Discarded future") {
      result = {};
    }
  }
  producer.set_value(42);
  REQUIRE(result.get() == 0);
  REQUIRE_FALSE(called);
}

TEST_CASE("FutureTest ThenSameFactoryClosed", "[wpiutil]") {
  PromiseFactory<int> factory;
  auto producer = factory.CreatePromise(factory.CreateRequest());
  auto capture = std::make_shared<int>(42);
  std::weak_ptr<int> lifetime = capture;
  bool called = false;
  auto result = producer.get_future().then(
      factory, [capture = std::move(capture), &called](int value) {
        called = true;
        return value + *capture;
      });

  factory.Close();
  REQUIRE(lifetime.expired());
  producer.set_value(7);
  REQUIRE(result.get() == 0);
  REQUIRE_FALSE(called);
}

TEST_CASE("FutureTest ThenDiscardedResult", "[wpiutil]") {
  promise<int> producer;
  bool called = false;
  producer.get_future().then([&](int) { called = true; });
  producer.set_value(42);
  REQUIRE(called);
}

TEMPLATE_TEST_CASE("FutureTest ThenException", "[wpiutil]", int, void) {
  promise<void> producer;
  bool called = false;
  future<TestType> result;
  {
    PromiseFactory<int> intermediate;
    PromiseFactory<TestType> destination;
    result = producer.get_future()
                 .then(intermediate,
                       []() -> int { throw std::runtime_error{"failure"}; })
                 .then(destination, [&](int) -> TestType {
                   called = true;
                   if constexpr (!std::is_void_v<TestType>) {
                     return 42;
                   }
                 });
    producer.set_value();
  }
  REQUIRE(result.is_ready());
  REQUIRE(result.wait_for(std::chrono::seconds{0}));
  REQUIRE_THROWS_AS(result.get(), std::runtime_error);
  REQUIRE_FALSE(result.valid());
  REQUIRE_FALSE(called);
}

TEST_CASE("FutureTest ThenReentrantCompletion", "[wpiutil]") {
  SECTION("Destroy source factory") {
    auto factory = std::make_unique<PromiseFactory<int>>();
    auto request = factory->CreateRequest();
    auto result = factory->CreateFuture(request).then([&](int value) {
      factory.reset();
      return value;
    });
    factory->SetValue(request, 42);
    REQUIRE(result.get() == 42);
  }
  SECTION("Destroy producer") {
    auto producer = std::make_unique<promise<int>>();
    auto result = producer->get_future().then([&](int value) {
      producer.reset();
      return value;
    });
    producer->set_value(42);
    REQUIRE(result.get() == 42);
  }
}

TEST_CASE("FutureTest ThenCompletesAfterClose", "[wpiutil]") {
  PromiseFactory<int> destination;
  promise<int> producer;
  std::barrier entered{2};
  std::barrier finish{2};
  auto result = producer.get_future().then(destination, [&](int value) {
    entered.arrive_and_wait();
    finish.arrive_and_wait();
    return value;
  });
  std::thread setter([&] { producer.set_value(42); });
  entered.arrive_and_wait();
  destination.Close();
  bool ready = result.wait_for(std::chrono::seconds{0});
  finish.arrive_and_wait();
  setter.join();

  REQUIRE_FALSE(ready);
  REQUIRE(result.get() == 0);
}

TEMPLATE_TEST_CASE("FutureTest ConcurrentFactoryDestruction", "[wpiutil]", int,
                   void) {
  for (int i = 0; i < 50; ++i) {
    auto factory = std::make_unique<PromiseFactory<TestType>>();
    auto producer = factory->CreatePromise(factory->CreateRequest());
    auto result = producer.get_future();
    std::barrier start{3};
    int value = 0;
    std::thread setter([&] {
      start.arrive_and_wait();
      if constexpr (std::is_void_v<TestType>) {
        producer.set_value();
      } else {
        producer.set_value(42);
      }
    });
    std::thread waiter([&] {
      start.arrive_and_wait();
      if constexpr (std::is_void_v<TestType>) {
        result.get();
      } else {
        value = result.get();
      }
    });
    start.arrive_and_wait();
    factory.reset();
    setter.join();
    waiter.join();

    REQUIRE((value == 0 || value == 42));
    REQUIRE_FALSE(result.valid());
  }
}

TEST_CASE("FutureTest WaitWhileRequestsChange", "[wpiutil]") {
  PromiseFactory<int> factory;
  auto request = factory.CreateRequest();
  auto result = factory.CreateFuture(request);
  int value = 0;
  std::thread waiter([&] { value = result.get(); });
  std::vector<future<int>> otherResults;
  for (int i = 0; i < 100; ++i) {
    auto other = factory.CreateRequest();
    otherResults.emplace_back(factory.CreateFuture(other));
    factory.SetValue(other, i);
  }
  bool matched = true;
  for (int i = 0; i < 100; ++i) {
    matched &= otherResults[i].get() == i;
  }
  factory.SetValue(request, 42);
  waiter.join();

  REQUIRE(matched);
  REQUIRE(value == 42);
}

}  // namespace wpi::util
