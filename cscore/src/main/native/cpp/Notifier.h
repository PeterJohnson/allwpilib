/*----------------------------------------------------------------------------*/
/* Copyright (c) 2015-2020 FIRST. All Rights Reserved.                        */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in the root directory of */
/* the project.                                                               */
/*----------------------------------------------------------------------------*/

#ifndef CSCORE_NOTIFIER_H_
#define CSCORE_NOTIFIER_H_

#include <functional>

#include <wpi/SafeThread.h>
#include <wpi/Signal.h>
#include <wpi/uv/Async.h>

#include "cscore_cpp.h"

namespace cs {

class Notifier {
  friend class NotifierTest;

 public:
  Notifier();
  Notifier(const Notifier&) = delete;
  Notifier& operator=(const Notifier&) = delete;
  ~Notifier();

  void Start();
  void Stop();

  void StartLoop(wpi::uv::Loop& loop);

  static bool destroyed() { return s_destroyed; }

  void SetOnStart(std::function<void()> on_start) { m_on_start = on_start; }
  void SetOnExit(std::function<void()> on_exit) { m_on_exit = on_exit; }

  int AddListener(std::function<void(const RawEvent& event)> callback,
                  int eventMask);
  void RemoveListener(int uid);

  void Notify(RawEvent&& event);
  void Notify(RawEvent::Kind eventKind) { Notify(RawEvent{eventKind}); }

  // notify on event loop
  wpi::sig::Signal<const RawEvent&> loopNotify;

 private:
  class Thread;
  wpi::SafeThreadOwner<Thread> m_owner;

  std::function<void()> m_on_start;
  std::function<void()> m_on_exit;
  static bool s_destroyed;

  using RawEventAsync = wpi::uv::Async<RawEvent>;
  std::shared_ptr<RawEventAsync> m_async;
};

}  // namespace cs

#endif  // CSCORE_NOTIFIER_H_
