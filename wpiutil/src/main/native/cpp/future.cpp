// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

#include "wpi/util/future.hpp"

namespace wpi::util {
namespace detail {

bool PromiseFactoryStateBase::IsActive() const noexcept {
  std::scoped_lock lock(m_resultMutex);
  return !m_closed;
}

void PromiseFactoryStateBase::Notify() noexcept {
  m_resultCv.notify_all();
}

}  // namespace detail

PromiseFactory<void>& PromiseFactory<void>::GetInstance() {
  static PromiseFactory<void> inst;
  return inst;
}

}  // namespace wpi::util
