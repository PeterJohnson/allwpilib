/*----------------------------------------------------------------------------*/
/* Copyright (c) 2018-2020 FIRST. All Rights Reserved.                        */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in the root directory of */
/* the project.                                                               */
/*----------------------------------------------------------------------------*/

#include "ServerImpl.h"

#include <algorithm>

#include <wpi/EventLoopRunner.h>
#include <wpi/HttpUtil.h>
#include <wpi/UrlParser.h>
#include <wpi/WebSocketServer.h>
#include <wpi/WebSocketServerConnection.h>
#include <wpi/json.h>
#include <wpi/raw_ostream.h>
#include <wpi/raw_uv_ostream.h>
#include <wpi/uv/Loop.h>
#include <wpi/uv/Tcp.h>
#include <wpi/uv/Timer.h>

#include "Instance.h"
#include "Log.h"
#include "Notifier.h"
#include "ServerStream.h"
#include "SourceImpl.h"
#include "Telemetry.h"

namespace uv = wpi::uv;

using namespace cs;

/*
 * REST API is documented in cameraserver.v1.yaml
 * WebSockets API is documented in cameraserver-ws.v1.yaml
 */

static constexpr size_t kMaxBodySize = 16 * 1024;

// static resources
namespace wpi {
StringRef GetResource_bootstrap_4_1_min_js_gz();
StringRef GetResource_coreui_2_1_min_css_gz();
StringRef GetResource_coreui_2_1_min_js_gz();
StringRef GetResource_feather_4_8_min_js_gz();
StringRef GetResource_jquery_3_3_slim_min_js_gz();
StringRef GetResource_popper_1_14_min_js_gz();
StringRef GetResource_wpilib_128_png();
}  // namespace wpi

namespace cs {
wpi::StringRef GetResource_cameraserver_css();
wpi::StringRef GetResource_cameraserver_js();
wpi::StringRef GetResource_cameraserver_v1_yaml();
wpi::StringRef GetResource_cameraserver_ws_v1_yaml();
wpi::StringRef GetResource_index_html();
}  // namespace cs

static constexpr struct {
  const char* filename;
  const char* contentType;
  wpi::StringRef (*getResource)();
  bool gzipped;
} gStaticResponses[] = {
    {"", "text/html", GetResource_index_html, false},
    {"index.html", "text/html", GetResource_index_html, false},
    {"cameraserver.css", "text/css", GetResource_cameraserver_css, false},
    {"cameraserver.js", "text/javascript", GetResource_cameraserver_js, false},
    {"cameraserver.v1.yaml", "application/yaml",
     GetResource_cameraserver_v1_yaml, false},
    {"cameraserver-ws.v1.yaml", "application/yaml",
     GetResource_cameraserver_ws_v1_yaml, false},
    {"bootstrap.min.js", "text/javascript",
     wpi::GetResource_bootstrap_4_1_min_js_gz, true},
    {"coreui.min.css", "text/css", wpi::GetResource_coreui_2_1_min_css_gz,
     true},
    {"coreui.min.js", "text/javascript", wpi::GetResource_coreui_2_1_min_js_gz,
     true},
    {"feather.min.js", "text/javascript",
     wpi::GetResource_feather_4_8_min_js_gz, true},
    {"jquery-3.3.1.slim.min.js", "text/javascript",
     wpi::GetResource_jquery_3_3_slim_min_js_gz, true},
    {"popper.min.js", "text/javascript", wpi::GetResource_popper_1_14_min_js_gz,
     true},
    {"wpilib.png", "image/png", wpi::GetResource_wpilib_128_png, false},
};

static wpi::json GetSourceInfoJson(CS_Source handle, SourceImpl& source) {
  CS_Status status = 0;
  wpi::json j = source.GetInfoJson(&status);
  auto& telemetry = Instance::GetInstance().GetTelemetry();
  j.emplace("actualFps", telemetry.GetAverageValue(
                             handle, CS_SOURCE_FRAMES_RECEIVED, &status));
  j.emplace("actualDataRate", telemetry.GetAverageValue(
                                  handle, CS_SOURCE_BYTES_RECEIVED, &status));
  return j;
}

class ServerImpl::Connection : public wpi::WebSocketServerConnection {
 public:
  explicit Connection(std::shared_ptr<uv::Stream> stream,
                      const sockaddr_storage& peer,
                      std::weak_ptr<ServerImpl> server, wpi::Logger& logger);
  ~Connection();

  void SendJsonResponse(const wpi::json& j);
  void SendMethodNotAllowed(wpi::StringRef methods);

 protected:
  void ProcessRequest() override;
  void ProcessWsUpgrade() override;

 private:
  wpi::json DoCreateSourceRequest(const wpi::json& j);

  void ProcessHttpCreateSourceRequest();
  void ProcessHttpSourceRequest(CS_Source handle,
                                std::shared_ptr<SourceImpl>& source,
                                CS_SourceKind kind, wpi::HttpPathRef path,
                                wpi::StringRef action);
  void ProcessHttpPropertiesRequest(PropertyContainer& container,
                                    wpi::HttpPathRef path);
  //void ProcessRtspRequest();
  void StartHttpStream(std::shared_ptr<SourceImpl> source);

  void InitWs();
  void SendWsError(int code, const wpi::Twine& message);
  void SendWsText(const wpi::json& j);

  void ProcessWsText(wpi::StringRef msg);
  void ProcessWsCreateSource(const wpi::json& req);
  void ProcessWsDeleteSource(const wpi::json& req);
  void ProcessWsGetSourceInfo(const wpi::json& req);
  void ProcessWsGetSourceStatus(const wpi::json& req);
  void ProcessWsGetSourceMode(const wpi::json& req);
  void ProcessWsSetSourceMode(const wpi::json& req);
  void ProcessWsGetSourceUsbPath(const wpi::json& req);
  void ProcessWsSetSourceUsbPath(const wpi::json& req);
  void ProcessWsGetSourceUrls(const wpi::json& req);
  void ProcessWsSetSourceUrls(const wpi::json& req);
  void ProcessWsGetSourceProperties(const wpi::json& req);
  void ProcessWsGetSourcePropertyInfo(const wpi::json& req);
  void ProcessWsGetSourceProperty(const wpi::json& req);
  void ProcessWsSetSourceProperty(const wpi::json& req);
  void ProcessWsGetServerPropertyInfo(const wpi::json& req);
  void ProcessWsGetServerProperty(const wpi::json& req);
  void ProcessWsSetServerProperty(const wpi::json& req);
  void ProcessWsGetServerStreamsPeriodic(const wpi::json& req);
  void ProcessWsStartStream(const wpi::json& req);

  void SendWsEvent(const RawEvent& event);
  void SendWsSourceUsbPath(CS_Source handle, const SourceImpl& source);
  void SendWsSourceUrls(CS_Source handle, const SourceImpl& source);
  void SendWsServerProperties();
  void SendWsServerStreams();
  void SendWsUsbCameraList();

  std::pair<CS_Source, std::shared_ptr<SourceData>> GetSource(
      const wpi::json& j);

  sockaddr_storage m_peer;
  std::weak_ptr<ServerImpl> m_server;

  std::shared_ptr<ServerStream> m_videoStream;

  wpi::sig::ScopedConnection m_eventConn;
  wpi::sig::ScopedConnection m_streamsChangedConn;
  std::shared_ptr<uv::Timer> m_streamsTimer;

  std::string m_requestBodyBuf;
  std::string m_requestBody;
  wpi::HttpQueryMap m_queryMap;

  wpi::Logger& m_logger;
};

ServerImpl::Connection::Connection(std::shared_ptr<uv::Stream> stream,
                                   const sockaddr_storage& peer,
                                   std::weak_ptr<ServerImpl> server,
                                   wpi::Logger& logger)
    : WebSocketServerConnection(stream, {"cameraserver"}),
      m_peer{peer},
      m_server{server},
      m_logger{logger} {
  //m_request.Reset(wpi::HttpParser::kHttpRtspRequest);
  m_request.body.connect([this](wpi::StringRef data, bool last) {
    if ((m_requestBodyBuf.size() + data.size()) > kMaxBodySize) {
      m_request.Abort();
      return SendError(413, "Payload Too Large");
    }
    m_requestBodyBuf += data;
  });
}

ServerImpl::Connection::~Connection() {
  if (m_streamsTimer) m_streamsTimer->Close();
}

void ServerImpl::Connection::ProcessRequest() {
  m_requestBody.swap(m_requestBodyBuf);
  m_requestBodyBuf.resize(0);

  //if (m_request.IsRtsp()) return ProcessRtspRequest();

  // parse URL
  wpi::UrlParser url{m_request.GetUrl(),
                     m_request.GetMethod() == wpi::HTTP_CONNECT};
  if (!url.IsValid()) return SendError(400);

  // parse path
  wpi::HttpPath path{url.GetPath()};
  if (path.empty()) return SendError(400);

  // parse query string
  if (url.HasQuery())
    m_queryMap = wpi::HttpQueryMap{url.GetQuery()};
  else
    m_queryMap = wpi::HttpQueryMap{};

  // get query string action= and other values
  wpi::SmallString<32> actionBuf;
  auto action = m_queryMap.Get("action", actionBuf).value_or("");

  auto& instance = Instance::GetInstance();
  CS_Status status = 0;
  const auto method = m_request.GetMethod();
  const bool isGET = method == wpi::HTTP_GET;
  const bool isPOST = method == wpi::HTTP_POST;

  if ((path.equals("") && action == "stream") || path.equals("stream.mjpg")) {
    // GET /?action=stream or /stream.mjpg: streams default stream
    if (isGET) {
      auto server = m_server.lock();
      if (!server) return SendError(503, "Service Unavailable");
      auto data =
          instance.FindSourceByName(server->GetConfig().defaultSource).second;
      if (data) return StartHttpStream(data->source);
    } else {
      return SendMethodNotAllowed("GET");
    }
  } else if (path.equals("stream")) {
    // POST /stream: streams default stream (backwards compatibility)
    if (isPOST) {
      auto server = m_server.lock();
      if (!server) return SendError(503, "Service Unavailable");
      auto data =
          instance.FindSourceByName(server->GetConfig().defaultSource).second;
      if (data) return StartHttpStream(data->source);
    } else {
      return SendMethodNotAllowed("POST");
    }
  } else if (path.startswith("sources")) {
    if (path.size() == 1) {
      if (isGET) {
        // GET /sources: gets list of sources
        return SendJsonResponse(instance.GetSourceBasicListJson());
      } else if (isPOST) {
        // POST /sources: creates source
        {
          auto server = m_server.lock();
          if (!server || !server->GetConfig().editSources)
            return SendError(403, "Forbidden");
        }
        return ProcessHttpCreateSourceRequest();
      } else {
        return SendMethodNotAllowed("GET, POST");
      }
    } else {
      // /sources/NAME: source NAME functions
      DEBUG3("source: " << path[1]);
      auto [handle, data] = instance.FindSourceByName(path[1]);
      if (data)
        return ProcessHttpSourceRequest(handle, data->source, data->kind,
                                        path.drop_front(2), action);
    }
  } else if (path.startswith("default")) {
    // /default: default source functions
    auto server = m_server.lock();
    if (!server) return SendError(503, "Service Unavailable");
    auto [handle, data] =
        instance.FindSourceByName(server->GetConfig().defaultSource);
    if (data)
      return ProcessHttpSourceRequest(handle, data->source, data->kind,
                                      path.drop_front(1), action);
  } else if (path.startswith("server")) {
    auto server = m_server.lock();
    if (!server) return SendError(503, "Service Unavailable");
    if (path.startswith(1, "properties")) {
      // /server/properties: server properties functions
      return ProcessHttpPropertiesRequest(*server, path.drop_front(2));
    } else if (path.equals(1, "streams")) {
      if (isGET) {
        // GET /server/streams: gets list of active streams
        return SendJsonResponse(server->GetStreamsInfoJson());
      } else {
        return SendMethodNotAllowed("GET");
      }
    }
  } else if (path.equals("usbcameras")) {
    if (isGET) {
      // GET /usbcameras: gets list of USB cameras
      return SendJsonResponse(EnumerateUsbCameras(&status));
    } else {
      return SendMethodNotAllowed("GET");
    }
  } else {
    // static responses
    for (const auto& response : gStaticResponses) {
      if (path.equals({response.filename})) {
        if (isGET)
          return SendStaticResponse(200, "OK", response.contentType,
                                    response.getResource(), response.gzipped);
        else
          return SendMethodNotAllowed("GET");
      }
    }
  }
  SendError(404, "Resource not found");
}

wpi::json ServerImpl::Connection::DoCreateSourceRequest(const wpi::json& j) {
  auto& id = j.at("id").get_ref<const std::string&>();
  auto& kind = j.at("kind").get_ref<const std::string&>();
  CS_Status status = 0;
  CS_Source source;
  if (kind == "usb") {
    // create USB source
    source = CreateUsbCameraPath(id, j.at("path").get_ref<const std::string&>(),
                                 &status);
  } else if (kind == "network") {
    // create network source
    auto& url = j.at("url");
    if (j.is_array())
      source =
          CreateNetworkSource(id, url.get<std::vector<std::string>>(), &status);
    else if (j.is_string())
      source =
          CreateNetworkSource(id, url.get_ref<const std::string&>(), &status);
    else
      return {{"type", "status"},
              {"code", 400},
              {"message", "url must be a string or array of string"}};
  } else {
    return {{"type", "status"},
            {"code", 400},
            {"message", "cannot create source of that kind"}};
  }
  if (source == 0 || status != 0)
    return {{"type", "status"},
            {"code", 400},
            {"message", "could not create source"}};

  // build response
  return {{"id", id},
          {"kind", kind},
          {"description", GetSourceDescription(source, &status)}};
}

void ServerImpl::Connection::ProcessHttpCreateSourceRequest() {
  try {
    auto r = DoCreateSourceRequest(wpi::json::parse(m_requestBody));
    if (r.count("code") != 0)
      return SendError(r.at("code").get<int>(),
                       r.at("message").get_ref<const std::string&>());
    return SendJsonResponse(r);
  } catch (const wpi::json::exception& e) {
    return SendError(400, e.what());
  }
}

void ServerImpl::Connection::ProcessHttpSourceRequest(
    CS_Source handle, std::shared_ptr<SourceImpl>& source, CS_SourceKind kind,
    wpi::HttpPathRef path, wpi::StringRef action) {
  CS_Status status = 0;
  const auto method = m_request.GetMethod();
  const bool isGET = method == wpi::HTTP_GET;
  const bool isPOST = method == wpi::HTTP_POST;
  const bool isPUT = method == wpi::HTTP_PUT;
  const bool isPATCH = method == wpi::HTTP_PATCH;

  if (path.empty()) {
    if (isGET) {
      // GET /?action=stream: stream
      if (action == "stream") return StartHttpStream(source);

      // GET /: Gets source information
      return SendJsonResponse(GetSourceInfoJson(handle, *source));
    } else if (method == wpi::HTTP_DELETE) {
      {
        auto server = m_server.lock();
        if (!server) return SendError(503, "Service Unavailable");
        if (!server->GetConfig().editSources)
          return SendError(403, "Forbidden");
      }
      Instance::GetInstance().DestroySource(handle);
      return SendResponse(204, "No Content", "", "");
    } else {
      return SendMethodNotAllowed("GET, DELETE");
    }
  } else if (path.equals("stream.mjpg")) {
    if (isGET)
      return StartHttpStream(source);
    else
      return SendMethodNotAllowed("GET");
  } else if (path.equals("stream")) {
    if (isPOST)
      return StartHttpStream(source);
    else
      return SendMethodNotAllowed("POST");
  } else if (path.equals("status")) {
    if (isGET)
      return SendJsonResponse({{"connected", source->IsConnected()}});
    else
      return SendMethodNotAllowed("GET");
  } else if (path.equals("mode")) {
    if (isGET) {
      return SendJsonResponse(source->GetVideoMode(&status));
    } else if (isPUT || isPATCH) {
      wpi::json j;
      VideoMode mode;
      try {
        j = wpi::json::parse(m_requestBody);
        mode = j.get<VideoMode>();
      } catch (const wpi::json::exception& e) {
        return SendError(400, e.what());
      }
      if (isPUT) {
        source->SetVideoMode(mode, &status);
      } else {
        if (j.count("pixelFormat") != 0)
          source->SetPixelFormat(
              static_cast<VideoMode::PixelFormat>(mode.pixelFormat), &status);
        if (j.count("width") != 0 && j.count("height") != 0)
          source->SetResolution(mode.width, mode.height, &status);
        if (j.count("fps") != 0)
          source->SetFPS(mode.fps, &status);
      }
      return SendResponse(204, "No Content", "", "");
    } else {
      return SendMethodNotAllowed("GET, PUT, PATCH");
    }
  } else if (path.startswith("properties")) {
    return ProcessHttpPropertiesRequest(*source, path.drop_front(1));
  } else if (kind == CS_SOURCE_USB && path.equals("usbpath")) {
    if (isGET) {
      return SendJsonResponse(GetUsbCameraPath(handle, &status));
    } else if (isPUT) {
      try {
        auto j = wpi::json::parse(m_requestBody);
        SetUsbCameraPath(handle, j.get_ref<const std::string&>(), &status);
      } catch (const wpi::json::exception& e) {
        return SendError(400, e.what());
      }
      return SendResponse(204, "No Content", "", "");
    } else {
      return SendMethodNotAllowed("GET, PUT");
    }
  } else if (kind == CS_SOURCE_NETWORK && path.equals("urls")) {
    if (isGET) {
      return SendJsonResponse(GetNetworkSourceUrls(handle, &status));
    } else if (isPUT) {
      try {
        auto j = wpi::json::parse(m_requestBody);
        SetNetworkSourceUrls(handle, j.get<std::vector<std::string>>(),
                             &status);
      } catch (const wpi::json::exception& e) {
        return SendError(400, e.what());
      }
      return SendResponse(204, "No Content", "", "");
    } else {
      return SendMethodNotAllowed("GET, PUT");
    }
  }
  SendError(404, "Resource not found");
}

void ServerImpl::Connection::ProcessHttpPropertiesRequest(
    PropertyContainer& container, wpi::HttpPathRef path) {
  CS_Status status = 0;
  const auto method = m_request.GetMethod();
  const bool isGET = method == wpi::HTTP_GET;
  const bool isPUT = method == wpi::HTTP_PUT;

  if (path.empty()) {
    if (isGET)
      return SendJsonResponse(container.GetPropertiesDetailJson(&status));
    else
      return SendMethodNotAllowed("GET");
  } else if (container.PropertyExists(path[0])) {
    int property = container.GetPropertyIndex(path[0]);
    if (path.size() == 1) {
      if (isGET) {
        return SendJsonResponse(
            container.GetPropertyDetailJson(property, &status));
      } else {
        return SendMethodNotAllowed("GET");
      }
    } else if (path.equals(1, "value")) {
      if (isGET) {
        return SendJsonResponse(container.GetPropertyValueJson(property,
                                                               &status));
      } else if (isPUT) {
        wpi::json j;
        try {
          j = wpi::json::parse(m_requestBody);
        } catch (const wpi::json::exception& e) {
          return SendError(400);
        }
        if (!container.SetPropertyValueJson(path[0], j, m_logger, "server",
                                            &status) ||
            status != 0)
          return SendError(400);
        else
          return SendResponse(204, "No Content", "", "");
      } else {
        return SendMethodNotAllowed("GET, PUT");
      }
    }
  }
  SendError(404, "Resource not found");
}

#if 0
void ServerImpl::Connection::SendRtspResponse(int code, const Twine& codeText,
                                              int cseq,
                                              const Twine& contentType,
                                              StringRef content,
                                              const Twine& extraHeader) {

}

void ServerImpl::Connection::ProcessRtspRequest() {
  switch (m_request.GetRtspMethod()) {
    case wpi::RTSP_DESCRIBE:
      break;
    case wpi::RTSP_GET_PARAMETER:
      break;
    case wpi::RTSP_OPTIONS:
      SendRtspResponse(
          200, "OK", cseq, "", "",
          "Public: DESCRIBE, GET_PARAMETER, OPTIONS, PAUSE, PLAY, SETUP, "
          "SET_PARAMETER, TEARDOWN\r\nSupported: play.basic");
      break;
    case wpi::RTSP_PAUSE:
      break;
    case wpi::RTSP_PLAY:
      break;
    case wpi::RTSP_SETUP:
      break;
    case wpi::RTSP_SET_PARAMETER:
      break;
    case wpi::RTSP_TEARDOWN:
      break;
  }
}
#endif
void ServerImpl::Connection::StartHttpStream(
    std::shared_ptr<SourceImpl> source) {
  // parse request parameters
  ServerStream::Config config;
  wpi::SmallString<32> valueBuf;

  if (auto val = m_queryMap.Get("resolution", valueBuf)) {
    auto [width, height] = val.value().split('x');
    if (width.getAsInteger(10, config.width))
      DEBUG0("resolution width '" << width << "' is not an integer");
    if (height.getAsInteger(10, config.height))
      DEBUG0("resolution height '" << width << "' is not an integer");
  }

  if (auto val = m_queryMap.Get("width", valueBuf)) {
    auto width = val.value();
    if (width.getAsInteger(10, config.width))
      DEBUG0("width '" << width << "' is not an integer");
  }

  if (auto val = m_queryMap.Get("height", valueBuf)) {
    auto height = val.value();
    if (height.getAsInteger(10, config.height))
      DEBUG0("height '" << height << "' is not an integer");
  }

  if (auto val = m_queryMap.Get("fps", valueBuf)) {
    auto fps = val.value();
    if (fps.getAsInteger(10, config.fps))
      DEBUG0("fps '" << fps << "' is not an integer");
  }

  // accept either "compression" or "quality" for quality
  if (auto val = m_queryMap.Get("compression", valueBuf)) {
    auto quality = val.value();
    if (quality.getAsInteger(10, config.quality))
      DEBUG0("compression '" << quality << "' is not an integer");
  } else if (auto val = m_queryMap.Get("quality", valueBuf)) {
    auto quality = val.value();
    if (quality.getAsInteger(10, config.quality))
      DEBUG0("quality '" << quality << "' is not an integer");
  }

  auto server = m_server.lock();
  if (!server) return SendError(503, "Service Unavailable");

  // Send initial multipart header
  wpi::SmallVector<uv::Buffer, 4> toSend;
  wpi::raw_uv_ostream os{toSend, 4096};
  BuildHeader(os, 200, "OK",
              wpi::Twine{"multipart/x-mixed-replace;boundary="} +
                  ServerHttpMjpegStream::kBoundary,
              0);
  SendData(os.bufs(), false);

  // start streaming
  m_videoStream = std::make_shared<ServerHttpMjpegStream>(
      m_stream, m_peer, server, std::move(source), config, m_logger);

  // ignore incoming data
  m_dataConn.disconnect();
}

void ServerImpl::Connection::SendJsonResponse(const wpi::json& j) {
  wpi::SmallString<256> resp;
  wpi::raw_svector_ostream os(resp);
  try {
    j.dump(os);
  } catch (const wpi::json::exception& e) {
    return SendError(500, "Internal Server Error");
  }
  SendResponse(200, "OK", "application/json", resp);
}

void ServerImpl::Connection::SendMethodNotAllowed(wpi::StringRef methods) {
  SendResponse(405, "Method Not Allowed", "", "",
               wpi::Twine{"Allow: "} + methods + "\r\n");
}

void ServerImpl::Connection::ProcessWsUpgrade() {
  // Connect the websocket open event to our connected event.
  m_websocket->open.connect_extended([this](auto conn, wpi::StringRef) {
    DEBUG0("websocket connected");
    InitWs();
    conn.disconnect();  // one-shot
  });
  m_websocket->text.connect(
      [this](wpi::StringRef msg, bool) { ProcessWsText(msg); });
}

void ServerImpl::Connection::InitWs() {
  m_eventConn =
      Instance::GetInstance().GetNotifier().loopNotify.connect_connection(
          [this](const RawEvent& event) { SendWsEvent(event); });
  if (auto server = m_server.lock()) {
    m_streamsChangedConn = server->streamsChanged.connect_connection(
        [this] { SendWsServerStreams(); });
  }
}

void ServerImpl::Connection::SendWsError(int code, const wpi::Twine& message) {
  SendWsText(
      {{"type", "status"}, {"code", code}, {"message", message.str()}});
}

void ServerImpl::Connection::SendWsText(const wpi::json& j) {
  wpi::SmallVector<uv::Buffer, 4> toSend;
  wpi::raw_uv_ostream os{toSend, 4096};
  os << j;
  m_websocket->SendText(toSend,
                        [](wpi::MutableArrayRef<uv::Buffer> bufs, uv::Error) {
                          for (auto&& buf : bufs) buf.Deallocate();
                        });
}

void ServerImpl::Connection::ProcessWsText(wpi::StringRef msg) {
  DEBUG4("ws: '" << msg << '\'');

  // parse
  wpi::json req;
  try {
    req = wpi::json::parse(msg);
  } catch (const wpi::json::parse_error& e) {
    DEBUG0("ws: parse error at byte " << e.byte << ": " << e.what());
    return;
  }

  // top level must be an object
  if (!req.is_object()) {
    DEBUG0("ws: not object");
    return;
  }

  // type
  wpi::StringRef type;
  try {
    type = req.at("type").get_ref<const std::string&>();
  } catch (const wpi::json::exception& e) {
    DEBUG0("ws: could not read type: " << e.what());
    return;
  }

  DEBUG4("ws: type: " << type);

  if (type == "getSourceList") {
    SendWsText({{"type", "sourceList"},
                {"sources", Instance::GetInstance().GetSourceBasicListJson()}});
  } else if (type == "createSource") {
    ProcessWsCreateSource(req);
  } else if (type == "deleteSource") {
    ProcessWsDeleteSource(req);
  } else if (type == "getSourceInfo") {
    ProcessWsGetSourceInfo(req);
  } else if (type == "getSourceStatus") {
    ProcessWsGetSourceStatus(req);
  } else if (type == "getSourceMode") {
    ProcessWsGetSourceMode(req);
  } else if (type == "setSourceMode") {
    ProcessWsSetSourceMode(req);
  } else if (type == "getSourceUsbPath") {
    ProcessWsGetSourceUsbPath(req);
  } else if (type == "setSourceUsbPath") {
    ProcessWsSetSourceUsbPath(req);
  } else if (type == "getSourceUrls") {
    ProcessWsGetSourceUrls(req);
  } else if (type == "setSourceUrls") {
    ProcessWsSetSourceUrls(req);
  } else if (type == "getSourceProperties") {
    ProcessWsGetSourceProperties(req);
  } else if (type == "getSourcePropertyInfo") {
    ProcessWsGetSourcePropertyInfo(req);
  } else if (type == "getSourceProperty") {
    ProcessWsGetSourceProperty(req);
  } else if (type == "setSourceProperty") {
    ProcessWsSetSourceProperty(req);
  } else if (type == "getServerProperties") {
    SendWsServerProperties();
  } else if (type == "getServerPropertyInfo") {
    ProcessWsGetServerPropertyInfo(req);
  } else if (type == "getServerProperty") {
    ProcessWsGetServerProperty(req);
  } else if (type == "setServerProperty") {
    ProcessWsSetServerProperty(req);
  } else if (type == "getServerStreams") {
    SendWsServerStreams();
  } else if (type == "getServerStreamsPeriodic") {
    ProcessWsGetServerStreamsPeriodic(req);
  } else if (type == "getUsbCameraList") {
    SendWsUsbCameraList();
  } else if (type == "startStream") {
    ProcessWsStartStream(req);
  } else if (type == "stopStream") {
    m_videoStream.reset();
  } else if (type == "getImage") {
    // TODO
  } else {
    DEBUG0("ws: unrecognized type " << type);
  }
}

void ServerImpl::Connection::ProcessWsCreateSource(const wpi::json& req) {
  auto sourceData = GetSource(req).second;
  if (!sourceData) return;
  auto server = m_server.lock();
  if (!server) return SendWsError(503, "Service Unavailable");
  if (!server->GetConfig().editSources)
    return SendWsError(403, "Cannot create or delete sources");
  try {
    auto j = DoCreateSourceRequest(req.at("source"));
    if (j.count("status") != 0) return SendWsText(j);
    // allow event to send actual create message
  } catch (const wpi::json::exception& e) {
    DEBUG0("ws: could not create source: " << e.what());
  }
}

void ServerImpl::Connection::ProcessWsDeleteSource(const wpi::json& req) {
  auto handle = GetSource(req).first;
  if (handle == 0) return;
  auto server = m_server.lock();
  if (!server) return SendWsError(503, "Service Unavailable");
  if (!server->GetConfig().editSources)
    return SendWsError(403, "Cannot create or delete sources");
  Instance::GetInstance().DestroySource(handle);
  // allow event to send actual delete message
}

void ServerImpl::Connection::ProcessWsGetSourceInfo(const wpi::json& req) {
  auto [handle, sourceData] = GetSource(req);
  if (!sourceData) return;
  SendWsText({{"type", "sourceInfo"},
              {"source", GetSourceInfoJson(handle, *sourceData->source)}});
}

void ServerImpl::Connection::ProcessWsGetSourceStatus(const wpi::json& req) {
  auto sourceData = GetSource(req).second;
  if (!sourceData) return;
  SendWsText({{"type", "sourceStatus"},
              {"sourceId", sourceData->source->GetName()},
              {"connected", sourceData->source->IsConnected()}});
}

void ServerImpl::Connection::ProcessWsGetSourceMode(const wpi::json& req) {
  auto sourceData = GetSource(req).second;
  if (!sourceData) return;
  CS_Status status = 0;
  SendWsText({{"type", "sourceMode"},
              {"sourceId", sourceData->source->GetName()},
              {"mode", sourceData->source->GetVideoMode(&status)}});
}

void ServerImpl::Connection::ProcessWsSetSourceMode(const wpi::json& req) {
  auto sourceData = GetSource(req).second;
  if (!sourceData) return;
  try {
    CS_Status status = 0;
    sourceData->source->SetVideoMode(req.at("mode").get<VideoMode>(), &status);
  } catch (const wpi::json::exception& e) {
    DEBUG0("ws: could not read mode: " << e.what());
    return;
  }
  // allow event to send actual mode change message
}

void ServerImpl::Connection::ProcessWsGetSourceUsbPath(const wpi::json& req) {
  auto [handle, sourceData] = GetSource(req);
  if (!sourceData) return;
  if (sourceData->kind != CS_SOURCE_USB) return;
  SendWsSourceUsbPath(handle, *sourceData->source);
}

void ServerImpl::Connection::ProcessWsSetSourceUsbPath(const wpi::json& req) {
  auto [handle, sourceData] = GetSource(req);
  if (!sourceData) return;
  if (sourceData->kind != CS_SOURCE_USB) return;
  CS_Status status = 0;
  try {
    SetUsbCameraPath(handle, req.at("path").get_ref<const std::string&>(),
                     &status);
  } catch (const wpi::json::exception& e) {
    DEBUG0("ws: could not read path: " << e.what());
    return;
  }
  SendWsSourceUsbPath(handle, *sourceData->source);
}

void ServerImpl::Connection::ProcessWsGetSourceUrls(const wpi::json& req) {
  auto [handle, sourceData] = GetSource(req);
  if (!sourceData) return;
  if (sourceData->kind != CS_SOURCE_NETWORK) return;
  SendWsSourceUrls(handle, *sourceData->source);
}

void ServerImpl::Connection::ProcessWsSetSourceUrls(const wpi::json& req) {
  auto [handle, sourceData] = GetSource(req);
  if (!sourceData) return;
  if (sourceData->kind != CS_SOURCE_NETWORK) return;
  CS_Status status = 0;
  try {
    SetNetworkSourceUrls(handle, req.at("urls").get<std::vector<std::string>>(),
                         &status);
  } catch (const wpi::json::exception& e) {
    DEBUG0("ws: could not read urls: " << e.what());
    return;
  }
  SendWsSourceUrls(handle, *sourceData->source);
}

void ServerImpl::Connection::ProcessWsGetSourceProperties(
    const wpi::json& req) {
  auto sourceData = GetSource(req).second;
  if (!sourceData) return;
  CS_Status status = 0;
  SendWsText(
      {{"type", "sourceProperties"},
       {"sourceId", sourceData->source->GetName()},
       {"properties", sourceData->source->GetPropertiesDetailJson(&status)}});
}

void ServerImpl::Connection::ProcessWsGetSourcePropertyInfo(
    const wpi::json& req) {
  auto sourceData = GetSource(req).second;
  if (!sourceData) return;
  try {
    auto& name = req.at("propertyId").get_ref<const std::string&>();
    if (!sourceData->source->PropertyExists(name)) return;
    int property = sourceData->source->GetPropertyIndex(name);
    CS_Status status = 0;
    SendWsText({{"type", "sourceProperty"},
                {"sourceId", sourceData->source->GetName()},
                {"property", sourceData->source->GetPropertyDetailJson(
                                 property, &status)}});
  } catch (const wpi::json::exception& e) {
    DEBUG0("ws: could not read propertyId: " << e.what());
    return;
  }
}

void ServerImpl::Connection::ProcessWsGetSourceProperty(const wpi::json& req) {
  auto sourceData = GetSource(req).second;
  if (!sourceData) return;
  try {
    auto& name = req.at("propertyId").get_ref<const std::string&>();
    if (!sourceData->source->PropertyExists(name)) return;
    int property = sourceData->source->GetPropertyIndex(name);
    CS_Status status = 0;
    SendWsText({{"type", "sourcePropertyValue"},
                {"sourceId", sourceData->source->GetName()},
                {"propertyId", name},
                {"value",
                 sourceData->source->GetPropertyValueJson(property, &status)}});
  } catch (const wpi::json::exception& e) {
    DEBUG0("ws: could not read propertyId: " << e.what());
    return;
  }
}

void ServerImpl::Connection::ProcessWsSetSourceProperty(const wpi::json& req) {
  auto sourceData = GetSource(req).second;
  if (!sourceData) return;
  try {
    auto& name = req.at("propertyId").get_ref<const std::string&>();
    if (!sourceData->source->PropertyExists(name)) return;
    CS_Status status = 0;
    sourceData->source->SetPropertyValueJson(name, req.at("value"), m_logger,
                                             "server", &status);
  } catch (const wpi::json::exception& e) {
    DEBUG0("ws: could not write property: " << e.what());
    return;
  }
  // allow event to send actual property value message
}

void ServerImpl::Connection::ProcessWsGetServerPropertyInfo(
    const wpi::json& req) {
  auto server = m_server.lock();
  if (!server) return;
  try {
    auto& name = req.at("propertyId").get_ref<const std::string&>();
    if (!server->PropertyExists(name)) return;
    int property = server->GetPropertyIndex(name);
    CS_Status status = 0;
    SendWsText(
        {{"type", "serverProperty"},
         {"property", server->GetPropertyDetailJson(property, &status)}});
  } catch (const wpi::json::exception& e) {
    DEBUG0("ws: could not read propertyId: " << e.what());
    return;
  }
}

void ServerImpl::Connection::ProcessWsGetServerProperty(const wpi::json& req) {
  auto server = m_server.lock();
  if (!server) return;
  try {
    auto& name = req.at("propertyId").get_ref<const std::string&>();
    if (!server->PropertyExists(name)) return;
    int property = server->GetPropertyIndex(name);
    CS_Status status = 0;
    SendWsText({{"type", "serverPropertyValue"},
                {"propertyId", name},
                {"value", server->GetPropertyValueJson(property, &status)}});
  } catch (const wpi::json::exception& e) {
    DEBUG0("ws: could not read propertyId: " << e.what());
    return;
  }
}

void ServerImpl::Connection::ProcessWsSetServerProperty(const wpi::json& req) {
  auto server = m_server.lock();
  if (!server) return;
  try {
    auto& name = req.at("propertyId").get_ref<const std::string&>();
    if (!server->PropertyExists(name)) return;
    CS_Status status = 0;
    server->SetPropertyValueJson(name, req.at("value"), m_logger, "server",
                                 &status);
  } catch (const wpi::json::exception& e) {
    DEBUG0("ws: could not write property: " << e.what());
    return;
  }
  // allow event to send actual property value message
}

void ServerImpl::Connection::ProcessWsGetServerStreamsPeriodic(
    const wpi::json& req) {
  double period;
  try {
    period = req.at("period").get<double>();
  } catch (const wpi::json::exception& e) {
    DEBUG0("ws: could not read period: " << e.what());
    return;
  }
  uv::Timer::Time repeat{static_cast<uint64_t>(period * 1000)};
  if (!m_streamsTimer) {
    m_streamsTimer = uv::Timer::Create(m_stream.GetLoopRef());
    m_streamsTimer->Start(uv::Timer::Time{0}, repeat);
    m_streamsTimer->timeout.connect([this] { SendWsServerStreams(); });
    m_streamsTimer->Unreference();
  } else {
    m_streamsTimer->SetRepeat(repeat);
  }
}

void ServerImpl::Connection::ProcessWsStartStream(const wpi::json& req) {
  auto server = m_server.lock();
  if (!server) return;
  auto sourceData = GetSource(req).second;
  if (!sourceData) return;

  ServerStream::Config config;
  try {
    auto& format = req.at("format").get_ref<const std::string&>();
    if (format != "mjpeg") return;
    if (req.count("width") != 0) config.width = req.at("width").get<int>();
    if (req.count("height") != 0) config.height = req.at("height").get<int>();
    if (req.count("fps") != 0) config.fps = req.at("fps").get<int>();
    if (req.count("quality") != 0)
      config.quality = req.at("quality").get<int>();
  } catch (const wpi::json::exception& e) {
    DEBUG0("ws: could not write property: " << e.what());
    return;
  }

  m_videoStream = std::make_shared<ServerWebSocketMjpegStream>(
      m_stream.GetLoopRef(), *m_websocket, m_peer, server, sourceData->source,
      config, m_logger);
}

void ServerImpl::Connection::SendWsSourceUsbPath(CS_Source handle,
                                                 const SourceImpl& source) {
  CS_Status status = 0;
  SendWsText({{"type", "sourceUsbPath"},
              {"sourceId", source.GetName()},
              {"path", GetUsbCameraPath(handle, &status)}});
}

void ServerImpl::Connection::SendWsSourceUrls(CS_Source handle,
                                              const SourceImpl& source) {
  CS_Status status = 0;
  SendWsText({{"type", "sourceUrls"},
              {"sourceId", source.GetName()},
              {"path", GetNetworkSourceUrls(handle, &status)}});
}

void ServerImpl::Connection::SendWsServerProperties() {
  auto server = m_server.lock();
  if (!server) return;
  CS_Status status = 0;
  SendWsText({{"type", "serverProperties"},
              {"properties", server->GetPropertiesDetailJson(&status)}});
}

void ServerImpl::Connection::SendWsServerStreams() {
  if (auto server = m_server.lock()) {
    SendWsText(
        {{"type", "serverStreams"}, {"streams", server->GetStreamsInfoJson()}});
  }
}

void ServerImpl::Connection::SendWsUsbCameraList() {
  CS_Status status = 0;
  SendWsText(
      {{"type", "usbCameraList"}, {"cameras", EnumerateUsbCameras(&status)}});
}

static bool PropertyEventValueToJson(wpi::json* j, const RawEvent& event) {
  switch (event.propertyKind) {
    case CS_PROP_BOOLEAN:
      j->emplace("value", event.value != 0);
      break;
    case CS_PROP_STRING:
      j->emplace("value", event.valueStr);
      break;
    case CS_PROP_INTEGER:
    case CS_PROP_ENUM:
      j->emplace("value", event.value);
      break;
    default:
      return false;
  }
  return true;
}

void ServerImpl::Connection::SendWsEvent(const RawEvent& event) {
  auto& inst = Instance::GetInstance();
  CS_Status status = 0;

  switch (event.kind) {
    case RawEvent::kSourceCreated: {
      auto sourceData = inst.GetSource(event.sourceHandle);
      if (!sourceData) return;
      auto j = inst.GetSourceBasicJson(*sourceData);
      j.emplace("type", "sourceCreated");
      SendWsText(j);
      break;
    }
    case RawEvent::kSourceDestroyed:
      SendWsText({{"type", "sourceDeleted"}, {"sourceId", event.name}});
      break;
    case RawEvent::kSourceConnected:
      SendWsText({{"type", "sourceStatus"},
                  {"sourceId", event.name},
                  {"status", {{"connected", true}}}});
      break;
    case RawEvent::kSourceDisconnected:
      SendWsText({{"type", "sourceStatus"},
                  {"sourceId", event.name},
                  {"status", {{"connected", false}}}});
      break;
    case RawEvent::kSourceVideoModesUpdated: {
      // TODO
      break;
    }
    case RawEvent::kSourceVideoModeChanged: {
      SendWsText({{"type", "sourceMode"},
                  {"sourceId", event.name},
                  {"mode", event.mode}});
      break;
    }
    case RawEvent::kSourcePropertyCreated: {
      auto sourceData = inst.GetSource(event.sourceHandle);
      if (!sourceData) return;
      SendWsText({{"type", "sourceProperty"},
                  {"sourceId", sourceData->source->GetName()},
                  {"property", sourceData->source->GetPropertyDetailJson(
                                   event.propertyHandle, &status)}});
      break;
    }
    case RawEvent::kSourcePropertyValueUpdated: {
      auto sourceData = inst.GetSource(event.sourceHandle);
      if (!sourceData) return;
      wpi::json j{{"type", "sourcePropertyValue"},
                  {"sourceId", sourceData->source->GetName()},
                  {"properyId", event.name}};
      if (!PropertyEventValueToJson(&j, event)) return;
      SendWsText(j);
      break;
    }
    case RawEvent::kSourcePropertyChoicesUpdated: {
      auto sourceData = inst.GetSource(event.sourceHandle);
      if (!sourceData) return;
      SendWsText({{"type", "sourcePropertyChoices"},
                  {"sourceId", sourceData->source->GetName()},
                  {"properyId", event.name},
                  {"choices", sourceData->source->GetEnumPropertyChoices(
                                  event.propertyHandle, &status)}});
      break;
    }
    case RawEvent::kServerPropertyCreated: {
      auto serverData = inst.GetServer(event.serverHandle);
      if (!serverData || serverData->server != m_server.lock()) return;
      SendWsText({{"type", "serverProperty"},
                  {"property", serverData->server->GetPropertyDetailJson(
                                   event.propertyHandle, &status)}});
      break;
    }
    case RawEvent::kServerPropertyValueUpdated: {
      auto serverData = inst.GetServer(event.serverHandle);
      if (!serverData || serverData->server != m_server.lock()) return;
      wpi::json j{{"type", "serverPropertyValue"}, {"properyId", event.name}};
      if (!PropertyEventValueToJson(&j, event)) return;
      SendWsText(j);
      break;
    }
    case RawEvent::kServerPropertyChoicesUpdated: {
      auto serverData = inst.GetServer(event.serverHandle);
      if (!serverData || serverData->server != m_server.lock()) return;
      SendWsText({{"type", "sourcePropertyChoices"},
                  {"properyId", event.name},
                  {"choices", serverData->server->GetEnumPropertyChoices(
                                  event.propertyHandle, &status)}});
      break;
    }
    default:
      break;
  }
}

std::pair<CS_Source, std::shared_ptr<SourceData>>
ServerImpl::Connection::GetSource(const wpi::json& j) {
  try {
    return Instance::GetInstance().FindSourceByName(
        j.at("sourceId").get_ref<const std::string&>());
  } catch (const wpi::json::exception& e) {
    ERROR("could not read source: " << e.what());
    return {0, nullptr};
  }
}

ServerImpl::ServerImpl(const ServerConfig& config,
                       wpi::EventLoopRunner& eventLoop, wpi::Logger& logger)
    : m_eventLoop(eventLoop), m_logger(logger), m_config(config) {}

ServerImpl::~ServerImpl() {
  m_eventLoop.ExecSync([=](uv::Loop& loop) {
    m_server->Close();
    m_newFrameAsync->Close();
  });
}

void ServerImpl::Start() {
  m_eventLoop.ExecAsync([this](uv::Loop& loop) {
    m_newFrameAsync = NewFrameAsync::Create(loop);
    if (!m_newFrameAsync) return;
    m_newFrameAsync->wakeup.connect([this](auto s, Frame frame) {
      if (auto stream = s.lock()) {
        stream->SendFrame(std::move(frame));
      }
    });

    m_server = uv::Tcp::Create(loop);

    // bind to listen address and port
    m_server->Bind(m_config.address, m_config.port);

    // when we get a connection, accept it and start reading
    m_server->connection.connect([this] {
      auto stream = m_server->Accept();
      if (!stream) return;
      DEBUG0("got a connection");

      // Close on error
      stream->error.connect([this, s = stream.get()](uv::Error err) {
        ERROR("stream error: " << err.str());
        s->Close();
      });

      auto conn = std::make_shared<Connection>(stream, stream->GetPeer(),
                                               weak_from_this(), m_logger);
      stream->SetData(conn);
    });

    // start listening for incoming connections
    m_server->Listen();

    INFO("listening on port " << m_config.port);
  });
}

ServerConfig ServerImpl::ParseConfig(wpi::StringRef config) {
  // TODO
  return ServerConfig{};
}

ServerConfig ServerImpl::ParseConfig(const wpi::json& config) {
  // TODO
  return ServerConfig{};
}

bool ServerImpl::SetConfigJson(const wpi::json& config, CS_Status* status) {
  // TODO
  return false;
}

wpi::json ServerImpl::GetConfigJson(CS_Status* status) const {
  // TODO
  return wpi::json{};
}

wpi::json ServerImpl::GetInfoJson(CS_Status* status) const {
  // TODO
  wpi::json j;
  return j;
}

wpi::json ServerImpl::GetStreamsInfoJson() const {
  wpi::json j = wpi::json::array();
  for (auto stream : m_streams) {
    wpi::json elem = stream->GetStreamInfo();
    if (!elem.empty()) j.emplace_back(std::move(elem));
  }
  return j;
}

void ServerImpl::UpdatePropertyValue(int property, bool setString, int value,
                                     const wpi::Twine& valueStr) {}

void ServerImpl::StartStream(ServerStream* conn) {
  m_streams.emplace_back(conn);
  streamsChanged();
}

void ServerImpl::StopStream(ServerStream* conn) {
  m_streams.erase(std::remove(m_streams.begin(), m_streams.end(), conn),
                  m_streams.end());
  streamsChanged();
}
