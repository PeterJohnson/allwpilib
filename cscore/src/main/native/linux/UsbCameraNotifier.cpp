/*----------------------------------------------------------------------------*/
/* Copyright (c) 2020 FIRST. All Rights Reserved.                             */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in the root directory of */
/* the project.                                                               */
/*----------------------------------------------------------------------------*/

#include "UsbCameraNotifier.h"

#include <wpi/uv/FsEvent.h>
#include <wpi/uv/Loop.h>
#include <wpi/uv/Timer.h>

using namespace cs;

class UsbCameraNotifier::Impl {
 public:
  wpi::sig::ScopedConnection timerConn;
};

UsbCameraNotifier::UsbCameraNotifier() {}

UsbCameraNotifier::~UsbCameraNotifier() {}

void UsbCameraNotifier::Start(wpi::uv::Loop& loop) {
  auto delayTimer = wpi::uv::Timer::Create(loop);
  m_impl->timerConn =
      delayTimer->timeout.connect_connection([this] { camerasChanged(); });
  delayTimer->Unreference();

  auto devEvents = wpi::uv::FsEvent::Create(loop);
  devEvents->fsEvent.connect([delayTimer](const char* fn, int flags) {
    if (wpi::StringRef(fn).startswith("video"))
      delayTimer->Start(wpi::uv::Timer::Time(200));
  });
  devEvents->Start("/dev");
  devEvents->Unreference();

  delayTimer->Start(wpi::uv::Timer::Time(200));
}
