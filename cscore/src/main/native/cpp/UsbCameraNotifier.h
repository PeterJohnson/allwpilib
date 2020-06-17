/*----------------------------------------------------------------------------*/
/* Copyright (c) 2020 FIRST. All Rights Reserved.                             */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in the root directory of */
/* the project.                                                               */
/*----------------------------------------------------------------------------*/

#ifndef CSCORE_USBCAMERANOTIFIER_H_
#define CSCORE_USBCAMERANOTIFIER_H_

#include <wpi/Signal.h>

namespace wpi {
namespace uv {
class Loop;
}  // namespace uv
}  // namespace wpi

namespace cs {

class UsbCameraNotifier {
 public:
  UsbCameraNotifier();
  ~UsbCameraNotifier();

  // must be called on the event loop
  void Start(wpi::uv::Loop& loop);

  wpi::sig::Signal<> camerasChanged;

 private:
  class Impl;
  std::unique_ptr<Impl> m_impl;
};

}  // namespace cs

#endif  // CSCORE_USBCAMERANOTIFIER_H_
