// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

#pragma once

#include <wpi/SymbolExports.h>
#include <wpi/protobuf/Protobuf.h>

#include "frc/geometry/Rectangle2d.h"

template <>
struct WPILIB_DLLEXPORT wpi::Protobuf<frc::Rectangle2d> {
  static google::protobuf::Message* New(google::protobuf::Arena* arena);
  static frc::Rectangle2d Unpack(const google::protobuf::Message& msg);
  static void Pack(google::protobuf::Message* msg,
                   const frc::Rectangle2d& value);
};
