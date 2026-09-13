// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

#include "wpi/net/WorkerThread.hpp"

#include <chrono>
#include <tuple>
#include <type_traits>

#include <catch2/catch_template_test_macros.hpp>
#include <catch2/catch_test_macros.hpp>

#include "wpi/net/EventLoopRunner.hpp"
#include "wpi/net/uv/Loop.hpp"
#include "wpi/util/condition_variable.hpp"
#include "wpi/util/mutex.hpp"

namespace wpi::net {

TEST_CASE("WorkerThreadTest Future", "[worker][thread]") {
  WorkerThread<int(bool)> worker;
  wpi::util::future<int> f =
      worker.QueueWork([](bool v) -> int { return v ? 1 : 2; }, true);
  REQUIRE(f.get() == 1);
}

TEST_CASE("WorkerThreadTest FutureVoid", "[worker][thread]") {
  int callbacks = 0;
  bool v3_check = false;
  WorkerThread<void(int)> worker;
  wpi::util::future<void> f = worker.QueueWork(
      [&](int v) {
        ++callbacks;
        v3_check = v == 3;
      },
      3);
  f.get();
  REQUIRE(callbacks == 1);
  REQUIRE(v3_check);
}

TEST_CASE("WorkerThreadTest Loop", "[worker][thread]") {
  wpi::util::mutex m;
  wpi::util::condition_variable cv;
  int callbacks = 0;
  bool v2_check = false;

  WorkerThread<int(bool)> worker;
  EventLoopRunner runner;
  runner.ExecSync([&](uv::Loop& loop) { worker.SetLoop(loop); });
  worker.QueueWorkThen([](bool v) -> int { return v ? 1 : 2; },
                       [&](int v2) {
                         std::scoped_lock lock{m};
                         ++callbacks;
                         cv.notify_all();
                         v2_check = v2 == 1;
                       },
                       true);
  auto f = worker.QueueWork([&](bool) -> int { return 2; }, true);
  REQUIRE(f.get() == 2);

  std::unique_lock lock{m};
  cv.wait(lock, [&] { return callbacks == 1; });
  REQUIRE(callbacks == 1);
  REQUIRE(v2_check);
}

TEST_CASE("WorkerThreadTest LoopVoid", "[worker][thread]") {
  wpi::util::mutex m;
  wpi::util::condition_variable cv;
  int callbacks = 0;

  WorkerThread<void(bool)> worker;
  EventLoopRunner runner;
  runner.ExecSync([&](uv::Loop& loop) { worker.SetLoop(loop); });
  worker.QueueWorkThen([](bool) {},
                       [&]() {
                         std::scoped_lock lock{m};
                         ++callbacks;
                         cv.notify_all();
                       },
                       true);
  auto f = worker.QueueWork([](bool) {}, true);
  f.get();

  std::unique_lock lock{m};
  cv.wait(lock, [&] { return callbacks == 1; });
  REQUIRE(callbacks == 1);
}

TEMPLATE_TEST_CASE("WorkerThreadTest FutureOutlivesWorker", "[worker][thread]",
                   int, void) {
  wpi::util::future<TestType> result;
  {
    WorkerThread<TestType()> worker;
    result = worker.QueueWork([]() -> TestType {
      if constexpr (!std::is_void_v<TestType>) {
        return 42;
      }
    });
    result.wait();
  }

  REQUIRE(result.is_ready());
  if constexpr (std::is_void_v<TestType>) {
    result.get();
  } else {
    REQUIRE(result.get() == 42);
  }
  REQUIRE_FALSE(result.valid());
}

TEST_CASE("WorkerThreadTest ShutdownCancelsQueuedWork", "[worker][thread]") {
  detail::WorkerThreadThread<int> thread;
  auto request = thread.m_promises.CreateRequest();
  auto result = thread.m_promises.CreateFuture(request);
  bool called = false;
  thread.m_requests.emplace_back(
      request,
      [&] {
        called = true;
        return 42;
      },
      std::tuple<>{});

  thread.Stop();
  thread.Main();

  // Thread exit must settle requests before the thread object is destroyed.
  REQUIRE_FALSE(called);
  REQUIRE_FALSE(result.wait_for(std::chrono::seconds{0}));
  REQUIRE(result.get() == 0);
}

}  // namespace wpi::net
