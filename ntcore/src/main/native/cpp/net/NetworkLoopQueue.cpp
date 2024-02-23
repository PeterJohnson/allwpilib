// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

#include "NetworkLoopQueue.h"

#include <wpi/Logger.h>

using namespace nt::net;

void NetworkLoopQueue::Append(ClientMessage&& msg) {
  std::scoped_lock lock{m_mutex};
  if (auto valueMsg = std::get_if<ClientValueMsg>(&msg.contents)) {
    m_valueSize += valueMsg->value.size();
    if (m_valueSize > m_maxValueSize) {
      if (!m_valueSizeErrored) {
        WPI_ERROR(m_logger, "NT: dropping value set due to memory limits");
        m_valueSizeErrored = true;
      }
      return;  // avoid potential out of memory
    }
  }
  if (m_queue.size() >= m_maxSize) {
    if (!m_sizeErrored) {
      WPI_ERROR(m_logger, "NT: dropping control message due to memory limits");
      m_sizeErrored = true;
    }
    return;  // avoid potential out of memory
  }
  m_queue.emplace_back(std::move(msg));
}
