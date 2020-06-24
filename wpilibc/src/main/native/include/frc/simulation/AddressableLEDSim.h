/*----------------------------------------------------------------------------*/
/* Copyright (c) 2018-2020 FIRST. All Rights Reserved.                        */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in the root directory of */
/* the project.                                                               */
/*----------------------------------------------------------------------------*/

#pragma once

#include <memory>
#include <utility>

#include <mockdata/AnalogGyroData.h>
#include <wpi/ArrayRef.h>

#include "CallbackStore.h"

namespace frc {
namespace sim {
class AddressableLEDSim {
 public:
  explicit AddressableLEDSim() { m_index = 0; }

  std::unique_ptr<CallbackStore> RegisterInitializedCallback(
      NotifyCallback callback, bool initialNotify) {
    auto store = std::make_unique<CallbackStore>(
        m_index, -1, callback,
        &HALSIM_RegisterAddressableLEDInitializedCallback);
    store->SetUid(HALSIM_RegisterAddressableLEDCallback(
        m_index, &CallbackStoreThunk, store.get(), initialNotify));
    return store;
  }

  bool GetInitialized() const {
    return HALSIM_GetAddressableLEDInitialized(m_index);
  }

  void SetInitialized(bool initialized) {
    HALSIM_SetAddressableLEDInitialized(m_index, initialized);
  }

  std::unique_ptr<CallbackStore> registerOutputPortCallback(
      NotifyCallback callback, bool initialNotify) {
    auto store = std::make_unique<CallbackStore>(
        m_index, -1, callback,
        &HALSIM_RegisterAddressableLEDOutputPortCallback);
    store->SetUid(HALSIM_RegisterAddressableLEDCallback(
        m_index, &CallbackStoreThunk, store.get(), initialNotify));
    return store;
  }

  int GetOutputPort() const {
    return HALSIM_GetAddressableLEDOutputPort(m_index);
  }

  void SetOutputPort(int outputPort) {
    HALSIM_SetAddressableLEDOutputPort(m_index, outputPort);
  }

  std::unique_ptr<CallbackStore> RegisterLengthCallback(NotifyCallback callback,
                                                        bool initialNotify) {
    auto store = std::make_unique<CallbackStore>(
        m_index, -1, callback, &HALSIM_RegisterAddressableLEDLengthCallback);
    store->SetUid(HALSIM_RegisterAddressableLEDCallback(
        m_index, &CallbackStoreThunk, store.get(), initialNotify));
    return store;
  }

  int GetLength() const { return HALSIM_GetAddressableLEDLength(m_index); }

  void SetLength(int length) {
    HALSIM_SetAddressableLEDLength(m_index, length);
  }

  std::unique_ptr<CallbackStore> RegisterRunningCallback(
      NotifyCallback callback, bool initialNotify) {
    auto store = std::make_unique<CallbackStore>(
        m_index, -1, callback, &HALSIM_RegisterAddressableLEDRunningCallback);
    store->SetUid(HALSIM_RegisterAddressableLEDCallback(
        m_index, &CallbackStoreThunk, store.get(), initialNotify));
    return store;
  }

  int GetRunning() const { return HALSIM_GetAddressableLEDRunning(m_index); }

  void SetRunning(bool running) {
    HALSIM_SetAddressableLEDRunning(m_index, running);
  }

  std::unique_ptr<CallbackStore> RegisterDataCallback(NotifyCallback callback,
                                                      bool initialNotify) {
    auto store = std::make_unique<CallbackStore>(
        m_index, -1, callback, &HALSIM_RegisterAddressableLEDRunningCallback);
    store->SetUid(HALSIM_RegisterAddressableLEDCallback(
        m_index, &CallbackStoreThunk, store.get(), initialNotify));
    return store;
  }

  int GetData() const { return HALSIM_GetAddressableLEDData(m_index); }
  // TODO: Fix bool to be java byte[]
  void SetData(bool data) { HALSIM_SetAddressableLEDData(m_index, data); }

  std::unique_ptr<CallbackStore> RegisterDataCallback(NotifyCallback callback,
                                                      bool initialNotify) {
    auto store = std::make_unique<CallbackStore>(
        m_index, -1, callback, &HALSIM_RegisterAddressableLEDRunningCallback);
    store->SetUid(HALSIM_RegisterAddressableLEDCallback(
        m_index, &CallbackStoreThunk, store.get(), initialNotify));
    return store;
  }

  int GetData() const { return HALSIM_GetAddressableLEDData(m_index); }
  void SetData(wpi::ArrayRef data) { HALSIM_SetAddressableLEDData(m_index, data); }
};
}  // namespace sim
}  // namespace frc