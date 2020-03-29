/*----------------------------------------------------------------------------*/
/* Copyright (c) 2016-2020 FIRST. All Rights Reserved.                        */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in the root directory of */
/* the project.                                                               */
/*----------------------------------------------------------------------------*/

#include "SourceImpl.h"

#include <algorithm>
#include <cstring>

#include <wpi/json.h>
#include <wpi/timestamp.h>

#include "FramePool.h"
#include "Instance.h"
#include "Log.h"

using namespace cs;

SourceImpl::SourceImpl(const wpi::Twine& name, wpi::Logger& logger)
    : m_logger(logger),
      m_framePool(Instance::GetInstance().GetFramePool()),
      m_name{name.str()} {
  m_frame = m_framePool.MakeEmptyFrame();
}

void SourceImpl::SetDescription(const wpi::Twine& description) {
  std::scoped_lock lock(m_mutex);
  m_description = description.str();
}

std::string SourceImpl::GetDescription() const {
  std::scoped_lock lock(m_mutex);
  return m_description;
}

wpi::StringRef SourceImpl::GetDescription(
    wpi::SmallVectorImpl<char>& buf) const {
  std::scoped_lock lock(m_mutex);
  buf.append(m_description.begin(), m_description.end());
  return wpi::StringRef{buf.data(), buf.size()};
}

void SourceImpl::SetConnected(bool isConnected) {
  bool wasConnected = m_isConnected.exchange(isConnected);
  if (wasConnected && !isConnected)
    disconnected();
  else if (!wasConnected && isConnected)
    connected();
}

uint64_t SourceImpl::GetCurFrameTime() {
  std::unique_lock lock{m_frameMutex};
  return m_frame.GetTime();
}

Frame SourceImpl::GetCurFrame() {
  std::unique_lock lock{m_frameMutex};
  return m_frame;
}

void SourceImpl::SetBrightness(int brightness, CS_Status* status) {
  *status = CS_INVALID_HANDLE;
}

int SourceImpl::GetBrightness(CS_Status* status) const {
  *status = CS_INVALID_HANDLE;
  return 0;
}

void SourceImpl::SetWhiteBalanceAuto(CS_Status* status) {
  *status = CS_INVALID_HANDLE;
}

void SourceImpl::SetWhiteBalanceHoldCurrent(CS_Status* status) {
  *status = CS_INVALID_HANDLE;
}

void SourceImpl::SetWhiteBalanceManual(int value, CS_Status* status) {
  *status = CS_INVALID_HANDLE;
}

void SourceImpl::SetExposureAuto(CS_Status* status) {
  *status = CS_INVALID_HANDLE;
}

void SourceImpl::SetExposureHoldCurrent(CS_Status* status) {
  *status = CS_INVALID_HANDLE;
}

void SourceImpl::SetExposureManual(int value, CS_Status* status) {
  *status = CS_INVALID_HANDLE;
}

VideoMode SourceImpl::GetVideoMode(CS_Status* status) const {
  if (!m_properties_cached && !CacheProperties(status)) return VideoMode{};
  std::scoped_lock lock(m_mutex);
  return m_mode;
}

bool SourceImpl::SetPixelFormat(VideoMode::PixelFormat pixelFormat,
                                CS_Status* status) {
  auto mode = GetVideoMode(status);
  if (!mode) return false;
  mode.pixelFormat = pixelFormat;
  return SetVideoMode(mode, status);
}

bool SourceImpl::SetResolution(int width, int height, CS_Status* status) {
  auto mode = GetVideoMode(status);
  if (!mode) return false;
  mode.width = width;
  mode.height = height;
  return SetVideoMode(mode, status);
}

bool SourceImpl::SetFPS(int fps, CS_Status* status) {
  auto mode = GetVideoMode(status);
  if (!mode) return false;
  mode.fps = fps;
  return SetVideoMode(mode, status);
}

std::vector<VideoMode> SourceImpl::EnumerateVideoModes(
    CS_Status* status) const {
  if (!m_properties_cached && !CacheProperties(status))
    return std::vector<VideoMode>{};
  std::scoped_lock lock(m_mutex);
  return m_videoModes;
}

void SourceImpl::PutFrame(Frame frame) {
  if (auto image = frame.GetExistingImage()) {
    // Update telemetry
    recordTelemetry(CS_SOURCE_FRAMES_RECEIVED, 1);
    recordTelemetry(CS_SOURCE_BYTES_RECEIVED,
                    static_cast<int64_t>(image->size()));
  }

  // Update frame
  {
    std::scoped_lock lock{m_frameMutex};
    m_frame = frame;
  }

  // Signal listeners
  newFrame(frame);
}

void SourceImpl::PutError(const wpi::Twine& msg, Frame::Time time) {
  PutFrame(m_framePool.MakeErrorFrame(msg, time));
}

void SourceImpl::UpdatePropertyValue(int property, bool setString, int value,
                                     const wpi::Twine& valueStr) {
  auto prop = GetProperty(property);
  if (!prop) return;

  if (setString)
    prop->SetValue(valueStr);
  else
    prop->SetValue(value);

  // Only notify updates after we've notified created
  if (m_properties_cached) propertyValueUpdated(property, *prop);
}

bool SourceImpl::SetConfigJson(const wpi::json& config, CS_Status* status) {
  bool ok = true;

  VideoMode mode;
  if (config.count("mode") != 0) {
    try {
      mode = config.at("mode").get<VideoMode>();
    } catch (const wpi::json::exception& e) {
      SWARNING("SetConfigJson: could not read video mode: " << e.what());
      ok = false;
    }
  }

  // if all of video mode is set, use SetVideoMode, otherwise piecemeal it
  if (mode.pixelFormat != VideoMode::kUnknown && mode.width != 0 &&
      mode.height != 0 && mode.fps != 0) {
    SINFO("SetConfigJson: setting video mode to pixelFormat "
          << mode.pixelFormat << ", width " << mode.width << ", height "
          << mode.height << ", fps " << mode.fps);
    SetVideoMode(mode, status);
  } else {
    if (mode.pixelFormat != cs::VideoMode::kUnknown) {
      SINFO("SetConfigJson: setting pixelFormat " << mode.pixelFormat);
      SetPixelFormat(static_cast<cs::VideoMode::PixelFormat>(mode.pixelFormat),
                     status);
    }
    if (mode.width != 0 && mode.height != 0) {
      SINFO("SetConfigJson: setting width " << mode.width << ", height "
                                            << mode.height);
      SetResolution(mode.width, mode.height, status);
    }
    if (mode.fps != 0) {
      SINFO("SetConfigJson: setting fps " << mode.fps);
      SetFPS(mode.fps, status);
    }
  }

  // brightness
  if (config.count("brightness") != 0) {
    try {
      int val = config.at("brightness").get<int>();
      SINFO("SetConfigJson: setting brightness to " << val);
      SetBrightness(val, status);
    } catch (const wpi::json::exception& e) {
      SWARNING("SetConfigJson: could not read brightness: " << e.what());
      ok = false;
    }
  }

  // white balance
  if (config.count("white balance") != 0) {
    try {
      auto& setting = config.at("white balance");
      if (setting.is_string()) {
        auto& str = setting.get_ref<const std::string&>();
        wpi::StringRef s(str);
        if (s.equals_lower("auto")) {
          SINFO("SetConfigJson: setting white balance to auto");
          SetWhiteBalanceAuto(status);
        } else if (s.equals_lower("hold")) {
          SINFO("SetConfigJson: setting white balance to hold current");
          SetWhiteBalanceHoldCurrent(status);
        } else {
          SWARNING("SetConfigJson: could not understand white balance value '"
                   << str << '\'');
          ok = false;
        }
      } else {
        int val = setting.get<int>();
        SINFO("SetConfigJson: setting white balance to " << val);
        SetWhiteBalanceManual(val, status);
      }
    } catch (const wpi::json::exception& e) {
      SWARNING("SetConfigJson: could not read white balance: " << e.what());
      ok = false;
    }
  }

  // exposure
  if (config.count("exposure") != 0) {
    try {
      auto& setting = config.at("exposure");
      if (setting.is_string()) {
        auto& str = setting.get_ref<const std::string&>();
        wpi::StringRef s(str);
        if (s.equals_lower("auto")) {
          SINFO("SetConfigJson: setting exposure to auto");
          SetExposureAuto(status);
        } else if (s.equals_lower("hold")) {
          SINFO("SetConfigJson: setting exposure to hold current");
          SetExposureHoldCurrent(status);
        } else {
          SWARNING("SetConfigJson: could not understand exposure value '"
                   << str << '\'');
          ok = false;
        }
      } else {
        int val = setting.get<int>();
        SINFO("SetConfigJson: setting exposure to " << val);
        SetExposureManual(val, status);
      }
    } catch (const wpi::json::exception& e) {
      SWARNING("SetConfigJson: could not read exposure: " << e.what());
      ok = false;
    }
  }

  // properties
  if (config.count("properties") != 0) {
    if (!SetPropertiesJson(config.at("properties"), m_logger, GetName(),
                           status))
      ok = false;
  }

  return ok;
}

static wpi::StringRef GetStrategyStr(int strategy) {
  switch (strategy) {
    case CS_CONNECTION_AUTO_MANAGE:
      return "auto";
    case CS_CONNECTION_KEEP_OPEN:
      return "open";
    case CS_CONNECTION_FORCE_CLOSE:
      return "close";
  }
  return {};
}

wpi::json SourceImpl::GetConfigJson(CS_Status* status) const {
  wpi::json j;

  j.emplace("id", GetName());
  auto strategyStr = GetStrategyStr(GetConnectionStrategy());
  if (!strategyStr.empty()) j.emplace("connectionStrategy", strategyStr);
  j.emplace("mode", GetVideoMode(status));

  // TODO: output brightness, white balance, and exposure?

  // properties
  wpi::json props = GetPropertiesJson(status);
  if (props.is_array()) j.emplace("properties", std::move(props));

  return j;
}

wpi::json SourceImpl::GetInfoJson(CS_Status* status) const {
  wpi::json j;

  j.emplace("id", GetName());
  j.emplace("description", GetDescription());
  auto strategyStr = GetStrategyStr(GetConnectionStrategy());
  if (!strategyStr.empty()) j.emplace("connectionStrategy", strategyStr);
  j.emplace("connected", IsConnected());
  j.emplace("mode", GetVideoMode(status));
  j.emplace("modes", EnumerateVideoModes(status));

  return j;
}
