// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

#pragma once

#include <span>
#include <string>
#include <vector>

#include "NetworkLoopQueue.h"
#include "ntcore_c.h"

namespace nt::net {

inline void ClientMessageQueue::SetMaxQueueSize(size_t size) {
  std::scoped_lock lock{m_mutex};
  m_maxSize = size;
}

inline void ClientMessageQueue::ReadQueue(std::vector<ClientMessage>* out) {
  std::scoped_lock lock{m_mutex};
  out->swap(m_queue);
  m_queue.resize(0);
  m_queue.reserve(out->capacity());  // keep the same running capacity
  m_valueSize = 0;
  m_sizeErrored = false;
  m_valueSizeErrored = false;
}

inline void ClientMessageQueue::ClearQueue() {
  std::scoped_lock lock{m_mutex};
  m_queue.resize(0);
  m_valueSize = 0;
  m_sizeErrored = false;
  m_valueSizeErrored = false;
}

inline void NetworkLoopQueue::Publish(NT_Publisher pubHandle,
                                      NT_Topic topicHandle,
                                      std::string_view name,
                                      std::string_view typeStr,
                                      const wpi::json& properties,
                                      const PubSubOptionsImpl& options) {
  Append(ClientMessage{PublishMsg{pubHandle, topicHandle, std::string{name},
                                  std::string{typeStr}, properties, options}});
}

inline void NetworkLoopQueue::Unpublish(NT_Publisher pubHandle,
                                        NT_Topic topicHandle) {
  Append(ClientMessage{UnpublishMsg{pubHandle, topicHandle}});
}

inline void NetworkLoopQueue::SetProperties(NT_Topic topicHandle,
                                            std::string_view name,
                                            const wpi::json& update) {
  Append(
      ClientMessage{SetPropertiesMsg{topicHandle, std::string{name}, update}});
}

inline void NetworkLoopQueue::Subscribe(NT_Subscriber subHandle,
                                        std::span<const std::string> topicNames,
                                        const PubSubOptionsImpl& options) {
  Append(ClientMessage{SubscribeMsg{
      subHandle, {topicNames.begin(), topicNames.end()}, options}});
}

inline void NetworkLoopQueue::Unsubscribe(NT_Subscriber subHandle) {
  Append(ClientMessage{UnsubscribeMsg{subHandle}});
}

inline void NetworkLoopQueue::SetValue(NT_Publisher pubHandle,
                                       const Value& value) {
  Append(ClientMessage{ClientValueMsg{pubHandle, value}});
}

}  // namespace nt::net
