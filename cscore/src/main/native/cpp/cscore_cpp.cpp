/*----------------------------------------------------------------------------*/
/* Copyright (c) 2016-2020 FIRST. All Rights Reserved.                        */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in the root directory of */
/* the project.                                                               */
/*----------------------------------------------------------------------------*/

#include "cscore_cpp.h"

#include <wpi/SmallString.h>
#include <wpi/hostname.h>
#include <wpi/json.h>

#include "Handle.h"
#include "Instance.h"
#include "Log.h"
#include "NetworkListener.h"
#include "Notifier.h"
#include "PropertyContainer.h"
#include "ServerImpl.h"
#include "SinkImpl.h"
#include "SourceImpl.h"
#include "Telemetry.h"

using namespace cs;

static std::shared_ptr<PropertyContainer> GetPropertyContainer(
    CS_Handle handle, Handle::Type* propertyType, CS_Status* status) {
  Handle h{handle};
  if (h.IsType(Handle::kSource)) {
    if (auto data = Instance::GetInstance().GetSource(handle)) {
      if (propertyType) *propertyType = Handle::kProperty;
      return data->source;
    }
  } else if (h.IsType(Handle::kSink)) {
    if (auto data = Instance::GetInstance().GetSink(handle)) {
      if (propertyType) *propertyType = Handle::kSinkProperty;
      return data->sink;
    }
  } else if (h.IsType(Handle::kServer)) {
    if (auto data = Instance::GetInstance().GetServer(handle)) {
      if (propertyType) *propertyType = Handle::kServerProperty;
      return data->server;
    }
  }
  *status = CS_INVALID_HANDLE;
  return nullptr;
}

static std::shared_ptr<PropertyContainer> GetProperty(
    CS_Property propertyHandle, int* propertyIndex, CS_Status* status) {
  Handle handle{propertyHandle};
  if (handle.IsType(Handle::kProperty)) {
    int i = handle.GetParentIndex();
    if (auto data =
            Instance::GetInstance().GetSource(Handle{i, Handle::kSource})) {
      *propertyIndex = handle.GetIndex();
      return data->source;
    }
  } else if (handle.IsType(Handle::kSinkProperty)) {
    int i = handle.GetParentIndex();
    if (auto data = Instance::GetInstance().GetSink(Handle{i, Handle::kSink})) {
      *propertyIndex = handle.GetIndex();
      return data->sink;
    }
  } else if (handle.IsType(Handle::kServerProperty)) {
    int i = handle.GetParentIndex();
    if (auto data =
            Instance::GetInstance().GetServer(Handle{i, Handle::kServer})) {
      *propertyIndex = handle.GetIndex();
      return data->server;
    }
  }
  *status = CS_INVALID_HANDLE;
  return nullptr;
}

namespace cs {

void to_json(wpi::json& j, const UsbCameraInfo& info) {
  j.emplace("dev", info.dev);
  j.emplace("path", info.path);
  j.emplace("name", info.name);
  j.emplace("otherPaths", info.otherPaths);
  if (info.vendorId != -1) j.emplace("vendorId", info.vendorId);
  if (info.productId != -1) j.emplace("productId", info.productId);
}

void from_json(const wpi::json& j, UsbCameraInfo& info) {
  info = UsbCameraInfo{};

  if (j.count("dev") != 0) info.dev = j.at("dev").get<int>();
  if (j.count("path") != 0)
    info.path = j.at("path").get_ref<const std::string&>();
  if (j.count("name") != 0)
    info.name = j.at("name").get_ref<const std::string&>();
  if (j.count("otherPaths") != 0)
    info.otherPaths = j.at("otherPaths").get<std::vector<std::string>>();
  if (j.count("vendorId") != 0) info.vendorId = j.at("vendorId").get<int>();
  if (j.count("productId") != 0) info.productId = j.at("productId").get<int>();
}

void to_json(wpi::json& j, const VideoMode& mode) {
  // pixel format
  wpi::StringRef pixelFormatStr;
  switch (mode.pixelFormat) {
    case VideoMode::kMJPEG:
      pixelFormatStr = "mjpeg";
      break;
    case VideoMode::kYUYV:
      pixelFormatStr = "yuyv";
      break;
    case VideoMode::kRGB565:
      pixelFormatStr = "rgb565";
      break;
    case VideoMode::kBGR:
      pixelFormatStr = "bgr";
      break;
    case VideoMode::kGray:
      pixelFormatStr = "gray";
      break;
    default:
      break;
  }
  if (!pixelFormatStr.empty()) j.emplace("pixelFormat", pixelFormatStr);

  // width
  if (mode.width != 0) j.emplace("width", mode.width);

  // height
  if (mode.height != 0) j.emplace("height", mode.height);

  // fps
  if (mode.fps != 0) j.emplace("fps", mode.fps);
}

void from_json(const wpi::json& j, VideoMode& mode) {
  mode = VideoMode{};

  // pixel format
  if (j.count("pixelFormat") != 0) {
    const auto& str = j.at("pixelFormat").get_ref<const std::string&>();
    wpi::StringRef s(str);
    if (s.equals_lower("mjpeg"))
      mode.pixelFormat = cs::VideoMode::kMJPEG;
    else if (s.equals_lower("yuyv"))
      mode.pixelFormat = cs::VideoMode::kYUYV;
    else if (s.equals_lower("rgb565"))
      mode.pixelFormat = cs::VideoMode::kRGB565;
    else if (s.equals_lower("bgr"))
      mode.pixelFormat = cs::VideoMode::kBGR;
    else if (s.equals_lower("gray"))
      mode.pixelFormat = cs::VideoMode::kGray;
    else
      throw wpi::json::other_error::create(
          600, "invalid pixelFormat " + wpi::Twine(s));
  }

  // width
  if (j.count("width") != 0) mode.width = j.at("width").get<unsigned int>();

  // height
  if (j.count("height") != 0) mode.height = j.at("height").get<unsigned int>();

  // fps
  if (j.count("fps") != 0) mode.fps = j.at("fps").get<unsigned int>();
}

//
// Property Functions
//

CS_PropertyKind GetPropertyKind(CS_Property property, CS_Status* status) {
  int propertyIndex;
  auto container = ::GetProperty(property, &propertyIndex, status);
  if (!container) return CS_PROP_NONE;
  return container->GetPropertyKind(propertyIndex);
}

std::string GetPropertyName(CS_Property property, CS_Status* status) {
  wpi::SmallString<128> buf;
  int propertyIndex;
  auto container = ::GetProperty(property, &propertyIndex, status);
  if (!container) return std::string{};
  return container->GetPropertyName(propertyIndex, buf, status);
}

wpi::StringRef GetPropertyName(CS_Property property,
                               wpi::SmallVectorImpl<char>& buf,
                               CS_Status* status) {
  int propertyIndex;
  auto container = ::GetProperty(property, &propertyIndex, status);
  if (!container) return wpi::StringRef{};
  return container->GetPropertyName(propertyIndex, buf, status);
}

int GetProperty(CS_Property property, CS_Status* status) {
  int propertyIndex;
  auto container = ::GetProperty(property, &propertyIndex, status);
  if (!container) return false;
  return container->GetProperty(propertyIndex, status);
}

void SetProperty(CS_Property property, int value, CS_Status* status) {
  int propertyIndex;
  auto container = ::GetProperty(property, &propertyIndex, status);
  if (!container) return;
  container->SetProperty(propertyIndex, value, status);
}

int GetPropertyMin(CS_Property property, CS_Status* status) {
  int propertyIndex;
  auto container = ::GetProperty(property, &propertyIndex, status);
  if (!container) return 0.0;
  return container->GetPropertyMin(propertyIndex, status);
}

int GetPropertyMax(CS_Property property, CS_Status* status) {
  int propertyIndex;
  auto container = ::GetProperty(property, &propertyIndex, status);
  if (!container) return 0.0;
  return container->GetPropertyMax(propertyIndex, status);
}

int GetPropertyStep(CS_Property property, CS_Status* status) {
  int propertyIndex;
  auto container = ::GetProperty(property, &propertyIndex, status);
  if (!container) return 0.0;
  return container->GetPropertyStep(propertyIndex, status);
}

int GetPropertyDefault(CS_Property property, CS_Status* status) {
  int propertyIndex;
  auto container = ::GetProperty(property, &propertyIndex, status);
  if (!container) return 0.0;
  return container->GetPropertyDefault(propertyIndex, status);
}

std::string GetStringProperty(CS_Property property, CS_Status* status) {
  wpi::SmallString<128> buf;
  int propertyIndex;
  auto container = ::GetProperty(property, &propertyIndex, status);
  if (!container) return std::string{};
  return container->GetStringProperty(propertyIndex, buf, status);
}

wpi::StringRef GetStringProperty(CS_Property property,
                                 wpi::SmallVectorImpl<char>& buf,
                                 CS_Status* status) {
  int propertyIndex;
  auto container = ::GetProperty(property, &propertyIndex, status);
  if (!container) return wpi::StringRef{};
  return container->GetStringProperty(propertyIndex, buf, status);
}

void SetStringProperty(CS_Property property, const wpi::Twine& value,
                       CS_Status* status) {
  int propertyIndex;
  auto container = ::GetProperty(property, &propertyIndex, status);
  if (!container) return;
  container->SetStringProperty(propertyIndex, value, status);
}

std::vector<std::string> GetEnumPropertyChoices(CS_Property property,
                                                CS_Status* status) {
  int propertyIndex;
  auto container = ::GetProperty(property, &propertyIndex, status);
  if (!container) return std::vector<std::string>{};
  return container->GetEnumPropertyChoices(propertyIndex, status);
}

//
// Source/Sink/Server Configuration Functions
//

CS_Property GetNodeProperty(CS_Handle node, const wpi::Twine& name,
                            CS_Status* status) {
  Handle::Type propertyType;
  auto container = ::GetPropertyContainer(node, &propertyType, status);
  if (!container) return 0;
  int property = container->GetPropertyIndex(name);
  if (property < 0) {
    *status = CS_INVALID_HANDLE;
    return 0;
  }
  return Handle{node, property, propertyType};
}

wpi::ArrayRef<CS_Property> EnumerateNodeProperties(
    CS_Handle node, wpi::SmallVectorImpl<CS_Property>& vec, CS_Status* status) {
  Handle::Type propertyType;
  auto container = ::GetPropertyContainer(node, &propertyType, status);
  if (!container) return 0;
  wpi::SmallVector<int, 32> properties_buf;
  for (auto property : container->EnumerateProperties(properties_buf, status))
    vec.push_back(Handle{node, property, propertyType});
  return vec;
}

bool SetNodeConfigJson(CS_Handle node, wpi::StringRef config,
                       CS_Status* status) {
  if (auto container = ::GetPropertyContainer(node, nullptr, status))
    return container->SetConfigJson(config, status);
  return false;
}

bool SetNodeConfigJson(CS_Handle node, const wpi::json& config,
                       CS_Status* status) {
  if (auto container = ::GetPropertyContainer(node, nullptr, status))
    return container->SetConfigJsonObject(config, status);
  return false;
}

std::string GetNodeConfigJson(CS_Handle node, CS_Status* status) {
  if (auto container = ::GetPropertyContainer(node, nullptr, status))
    return container->GetConfigJson(status);
  return std::string{};
}

wpi::json GetNodeConfigJsonObject(CS_Handle node, CS_Status* status) {
  if (auto container = ::GetPropertyContainer(node, nullptr, status))
    return container->GetConfigJsonObject(status);
  return wpi::json{};
}

CS_Handle CopyNode(CS_Handle node, CS_Status* status) {
  if (node == 0) return 0;
  auto& inst = Instance::GetInstance();
  Handle handle{node};
  if (handle.IsType(Handle::kSource)) {
    if (auto data = inst.GetSource(node)) {
      data->refCount++;
      return node;
    }
  } else if (handle.IsType(Handle::kSink)) {
    if (auto data = inst.GetSink(node)) {
      data->refCount++;
      return node;
    }
  } else if (handle.IsType(Handle::kServer)) {
    if (auto data = inst.GetServer(node)) {
      data->refCount++;
      return node;
    }
  }
  *status = CS_INVALID_HANDLE;
  return 0;
}

void ReleaseNode(CS_Handle node, CS_Status* status) {
  if (node == 0) return;
  auto& inst = Instance::GetInstance();
  Handle handle{node};
  if (handle.IsType(Handle::kSource)) {
    if (auto data = inst.GetSource(node)) {
      if (data->refCount-- == 0) inst.DestroySource(node);
      return;
    }
  } else if (handle.IsType(Handle::kSink)) {
    if (auto data = inst.GetSink(node)) {
      if (data->refCount-- == 0) inst.DestroySink(node);
      return;
    }
  } else if (handle.IsType(Handle::kServer)) {
    if (auto data = inst.GetServer(node)) {
      if (data->refCount-- == 0) inst.StopServer(node);
      return;
    }
  }
  *status = CS_INVALID_HANDLE;
}

bool IsNodeEnabled(CS_Handle node, CS_Status* status) {
  auto& inst = Instance::GetInstance();
  Handle handle{node};
  if (handle.IsType(Handle::kSource)) {
    if (auto data = inst.GetSource(node)) return data->source->IsEnabled();
  } else if (handle.IsType(Handle::kSink)) {
    if (auto data = inst.GetSink(node)) return data->sink->IsEnabled();
  } else if (handle.IsType(Handle::kServer)) {
    return true;
  }
  *status = CS_INVALID_HANDLE;
  return false;
}

//
// Source Functions
//

CS_SourceKind GetSourceKind(CS_Source source, CS_Status* status) {
  auto data = Instance::GetInstance().GetSource(source);
  if (!data) {
    *status = CS_INVALID_HANDLE;
    return CS_SOURCE_UNKNOWN;
  }
  return data->kind;
}

std::string GetSourceName(CS_Source source, CS_Status* status) {
  auto data = Instance::GetInstance().GetSource(source);
  if (!data) {
    *status = CS_INVALID_HANDLE;
    return std::string{};
  }
  return data->source->GetName();
}

wpi::StringRef GetSourceName(CS_Source source, wpi::SmallVectorImpl<char>& buf,
                             CS_Status* status) {
  auto data = Instance::GetInstance().GetSource(source);
  if (!data) {
    *status = CS_INVALID_HANDLE;
    return wpi::StringRef{};
  }
  return data->source->GetName();
}

std::string GetSourceDescription(CS_Source source, CS_Status* status) {
  auto data = Instance::GetInstance().GetSource(source);
  if (!data) {
    *status = CS_INVALID_HANDLE;
    return std::string{};
  }
  wpi::SmallString<128> buf;
  return data->source->GetDescription(buf);
}

wpi::StringRef GetSourceDescription(CS_Source source,
                                    wpi::SmallVectorImpl<char>& buf,
                                    CS_Status* status) {
  auto data = Instance::GetInstance().GetSource(source);
  if (!data) {
    *status = CS_INVALID_HANDLE;
    return wpi::StringRef{};
  }
  return data->source->GetDescription(buf);
}

uint64_t GetSourceLastFrameTime(CS_Source source, CS_Status* status) {
  auto data = Instance::GetInstance().GetSource(source);
  if (!data) {
    *status = CS_INVALID_HANDLE;
    return 0;
  }
  return data->source->GetCurFrameTime();
}

void SetSourceConnectionStrategy(CS_Source source,
                                 CS_ConnectionStrategy strategy,
                                 CS_Status* status) {
  auto data = Instance::GetInstance().GetSource(source);
  if (!data) {
    *status = CS_INVALID_HANDLE;
    return;
  }
  data->source->SetConnectionStrategy(strategy);
}

bool IsSourceConnected(CS_Source source, CS_Status* status) {
  auto data = Instance::GetInstance().GetSource(source);
  if (!data) {
    *status = CS_INVALID_HANDLE;
    return false;
  }
  return data->source->IsConnected();
}

VideoMode GetSourceVideoMode(CS_Source source, CS_Status* status) {
  auto data = Instance::GetInstance().GetSource(source);
  if (!data) {
    *status = CS_INVALID_HANDLE;
    return VideoMode{};
  }
  return data->source->GetVideoMode(status);
}

bool SetSourceVideoMode(CS_Source source, const VideoMode& mode,
                        CS_Status* status) {
  auto data = Instance::GetInstance().GetSource(source);
  if (!data) {
    *status = CS_INVALID_HANDLE;
    return false;
  }
  return data->source->SetVideoMode(mode, status);
}

bool SetSourcePixelFormat(CS_Source source, VideoMode::PixelFormat pixelFormat,
                          CS_Status* status) {
  auto data = Instance::GetInstance().GetSource(source);
  if (!data) {
    *status = CS_INVALID_HANDLE;
    return false;
  }
  return data->source->SetPixelFormat(pixelFormat, status);
}

bool SetSourceResolution(CS_Source source, int width, int height,
                         CS_Status* status) {
  auto data = Instance::GetInstance().GetSource(source);
  if (!data) {
    *status = CS_INVALID_HANDLE;
    return false;
  }
  return data->source->SetResolution(width, height, status);
}

bool SetSourceFPS(CS_Source source, int fps, CS_Status* status) {
  auto data = Instance::GetInstance().GetSource(source);
  if (!data) {
    *status = CS_INVALID_HANDLE;
    return false;
  }
  return data->source->SetFPS(fps, status);
}

std::vector<VideoMode> EnumerateSourceVideoModes(CS_Source source,
                                                 CS_Status* status) {
  auto data = Instance::GetInstance().GetSource(source);
  if (!data) {
    *status = CS_INVALID_HANDLE;
    return std::vector<VideoMode>{};
  }
  return data->source->EnumerateVideoModes(status);
}

wpi::ArrayRef<CS_Sink> EnumerateSourceSinks(CS_Source source,
                                            wpi::SmallVectorImpl<CS_Sink>& vec,
                                            CS_Status* status) {
  auto& inst = Instance::GetInstance();
  auto data = inst.GetSource(source);
  if (!data) {
    *status = CS_INVALID_HANDLE;
    return wpi::ArrayRef<CS_Sink>{};
  }
  return inst.EnumerateSourceSinks(source, vec);
}

//
// Camera Source Common Property Fuctions
//

void SetCameraBrightness(CS_Source source, int brightness, CS_Status* status) {
  auto data = Instance::GetInstance().GetSource(source);
  if (!data) {
    *status = CS_INVALID_HANDLE;
    return;
  }
  data->source->SetBrightness(brightness, status);
}

int GetCameraBrightness(CS_Source source, CS_Status* status) {
  auto data = Instance::GetInstance().GetSource(source);
  if (!data) {
    *status = CS_INVALID_HANDLE;
    return 0;
  }
  return data->source->GetBrightness(status);
}

void SetCameraWhiteBalanceAuto(CS_Source source, CS_Status* status) {
  auto data = Instance::GetInstance().GetSource(source);
  if (!data) {
    *status = CS_INVALID_HANDLE;
    return;
  }
  data->source->SetWhiteBalanceAuto(status);
}

void SetCameraWhiteBalanceHoldCurrent(CS_Source source, CS_Status* status) {
  auto data = Instance::GetInstance().GetSource(source);
  if (!data) {
    *status = CS_INVALID_HANDLE;
    return;
  }
  data->source->SetWhiteBalanceHoldCurrent(status);
}

void SetCameraWhiteBalanceManual(CS_Source source, int value,
                                 CS_Status* status) {
  auto data = Instance::GetInstance().GetSource(source);
  if (!data) {
    *status = CS_INVALID_HANDLE;
    return;
  }
  data->source->SetWhiteBalanceManual(value, status);
}

void SetCameraExposureAuto(CS_Source source, CS_Status* status) {
  auto data = Instance::GetInstance().GetSource(source);
  if (!data) {
    *status = CS_INVALID_HANDLE;
    return;
  }
  data->source->SetExposureAuto(status);
}

void SetCameraExposureHoldCurrent(CS_Source source, CS_Status* status) {
  auto data = Instance::GetInstance().GetSource(source);
  if (!data) {
    *status = CS_INVALID_HANDLE;
    return;
  }
  data->source->SetExposureHoldCurrent(status);
}

void SetCameraExposureManual(CS_Source source, int value, CS_Status* status) {
  auto data = Instance::GetInstance().GetSource(source);
  if (!data) {
    *status = CS_INVALID_HANDLE;
    return;
  }
  data->source->SetExposureManual(value, status);
}

//
// Sink Functions
//

CS_SinkKind GetSinkKind(CS_Sink sink, CS_Status* status) {
  auto data = Instance::GetInstance().GetSink(sink);
  if (!data) {
    *status = CS_INVALID_HANDLE;
    return CS_SINK_UNKNOWN;
  }
  return data->kind;
}

std::string GetSinkName(CS_Sink sink, CS_Status* status) {
  auto data = Instance::GetInstance().GetSink(sink);
  if (!data) {
    *status = CS_INVALID_HANDLE;
    return std::string{};
  }
  return data->sink->GetName();
}

wpi::StringRef GetSinkName(CS_Sink sink, wpi::SmallVectorImpl<char>& buf,
                           CS_Status* status) {
  auto data = Instance::GetInstance().GetSink(sink);
  if (!data) {
    *status = CS_INVALID_HANDLE;
    return wpi::StringRef{};
  }
  return data->sink->GetName();
}

std::string GetSinkDescription(CS_Sink sink, CS_Status* status) {
  auto data = Instance::GetInstance().GetSink(sink);
  if (!data) {
    *status = CS_INVALID_HANDLE;
    return std::string{};
  }
  wpi::SmallString<128> buf;
  return data->sink->GetDescription(buf);
}

wpi::StringRef GetSinkDescription(CS_Sink sink, wpi::SmallVectorImpl<char>& buf,
                                  CS_Status* status) {
  auto data = Instance::GetInstance().GetSink(sink);
  if (!data) {
    *status = CS_INVALID_HANDLE;
    return wpi::StringRef{};
  }
  return data->sink->GetDescription(buf);
}

void SetSinkSource(CS_Sink sink, CS_Source source, CS_Status* status) {
  auto data = Instance::GetInstance().GetSink(sink);
  if (!data) {
    *status = CS_INVALID_HANDLE;
    return;
  }
  if (source == 0) {
    data->sink->SetSource(nullptr);
  } else {
    auto sourceData = Instance::GetInstance().GetSource(source);
    if (!sourceData) {
      *status = CS_INVALID_HANDLE;
      return;
    }
    data->sink->SetSource(sourceData->source);
  }
  data->sourceHandle.store(source);
}

CS_Source GetSinkSource(CS_Sink sink, CS_Status* status) {
  auto data = Instance::GetInstance().GetSink(sink);
  if (!data) {
    *status = CS_INVALID_HANDLE;
    return 0;
  }
  return data->sourceHandle.load();
}

CS_Property GetSinkSourceProperty(CS_Sink sink, const wpi::Twine& name,
                                  CS_Status* status) {
  auto data = Instance::GetInstance().GetSink(sink);
  if (!data) {
    *status = CS_INVALID_HANDLE;
    return 0;
  }
  return GetNodeProperty(data->sourceHandle.load(), name, status);
}

//
// Server Functions
//

CS_Server StartServer(const ServerConfig& config, CS_Status* status) {
  return Instance::GetInstance().StartServer(config);
}

CS_Server StartServerJson(wpi::StringRef config, CS_Status* status) {
  return Instance::GetInstance().StartServer(config);
}

CS_Server StartServerJson(const wpi::json& config, CS_Status* status) {
  return Instance::GetInstance().StartServer(config);
}

void StopServer(CS_Server server, CS_Status* status) {
  Instance::GetInstance().StopServer(server);
}

void StopAllServers() { Instance::GetInstance().StopAllServers(); }

//
// Listener Functions
//

void SetListenerOnStart(std::function<void()> onStart) {
  Instance::GetInstance().GetNotifier().SetOnStart(onStart);
}

void SetListenerOnExit(std::function<void()> onExit) {
  Instance::GetInstance().GetNotifier().SetOnExit(onExit);
}

CS_Listener AddListener(std::function<void(const RawEvent& event)> callback,
                        int eventMask, bool immediateNotify,
                        CS_Status* status) {
  auto& inst = Instance::GetInstance();
  auto& notifier = inst.GetNotifier();
  int uid = notifier.AddListener(callback, eventMask);
  if ((eventMask & CS_NETWORK_INTERFACES_CHANGED) != 0) {
    // start network interface event listener
    inst.StartNetworkListener(immediateNotify);
  }
  if (immediateNotify) {
    // TODO
  }
  return Handle{uid, Handle::kListener};
}

void RemoveListener(CS_Listener handle, CS_Status* status) {
  int uid = Handle{handle}.GetTypedIndex(Handle::kListener);
  if (uid < 0) {
    *status = CS_INVALID_HANDLE;
    return;
  }
  Instance::GetInstance().GetNotifier().RemoveListener(uid);
}

bool NotifierDestroyed() { return Notifier::destroyed(); }

//
// Telemetry Functions
//
void SetTelemetryPeriod(double seconds) {
  auto& telemetry = Instance::GetInstance().GetTelemetry();
  telemetry.Start();
  telemetry.SetPeriod(seconds);
}

double GetTelemetryElapsedTime() {
  return Instance::GetInstance().GetTelemetry().GetElapsedTime();
}

int64_t GetTelemetryValue(CS_Handle handle, CS_TelemetryKind kind,
                          CS_Status* status) {
  return Instance::GetInstance().GetTelemetry().GetValue(handle, kind, status);
}

double GetTelemetryAverageValue(CS_Handle handle, CS_TelemetryKind kind,
                                CS_Status* status) {
  return Instance::GetInstance().GetTelemetry().GetAverageValue(handle, kind,
                                                                status);
}

//
// Logging Functions
//
void SetLogger(LogFunc func, unsigned int min_level) {
  auto& logger = Instance::GetInstance().GetLogger();
  logger.SetLogger(func);
  logger.set_min_level(min_level);
}

void SetDefaultLogger(unsigned int min_level) {
  auto& inst = Instance::GetInstance();
  inst.SetDefaultLogger();
  inst.GetLogger().set_min_level(min_level);
}

//
// Shutdown Function
//
void Shutdown() { Instance::GetInstance().Shutdown(); }

//
// Utility Functions
//

wpi::ArrayRef<CS_Source> EnumerateSourceHandles(
    wpi::SmallVectorImpl<CS_Source>& vec, CS_Status* status) {
  return Instance::GetInstance().EnumerateSourceHandles(vec);
}

wpi::ArrayRef<CS_Sink> EnumerateSinkHandles(wpi::SmallVectorImpl<CS_Sink>& vec,
                                            CS_Status* status) {
  return Instance::GetInstance().EnumerateSinkHandles(vec);
}

std::string GetHostname() { return wpi::GetHostname(); }

}  // namespace cs
