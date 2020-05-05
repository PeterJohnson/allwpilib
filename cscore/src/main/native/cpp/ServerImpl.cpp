/*----------------------------------------------------------------------------*/
/* Copyright (c) 2018-2020 FIRST. All Rights Reserved.                        */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in the root directory of */
/* the project.                                                               */
/*----------------------------------------------------------------------------*/

#include "ServerImpl.h"

#include <wpi/EventLoopRunner.h>
#include <wpi/WebSocketServerConnection.h>
#include <wpi/UrlParser.h>
#include <wpi/WebSocketServer.h>
#include <wpi/json.h>
#include <wpi/raw_ostream.h>
#include <wpi/uv/Loop.h>
#include <wpi/uv/Tcp.h>

#include "Log.h"
#include "SourceImpl.h"

namespace uv = wpi::uv;

using namespace cs;

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

namespace {
class MyHttpConnection : public wpi::WebSocketServerConnection {
 public:
  explicit MyHttpConnection(std::shared_ptr<uv::Stream> stream,
                            std::weak_ptr<ServerImpl> server);

 protected:
  void ProcessRequest() override;
  void ProcessWsUpgrade() override;

  std::weak_ptr<ServerImpl> m_server;

  std::weak_ptr<SourceImpl> m_defaultSource;

  std::shared_ptr<SourceImpl> m_source;
  wpi::sig::ScopedConnection m_onNewFrame;
};
}  // namespace

MyHttpConnection::MyHttpConnection(std::shared_ptr<uv::Stream> stream,
                                   std::weak_ptr<ServerImpl> server)
    : WebSocketServerConnection(stream, {"cameraserver"}), m_server{server} {}

void MyHttpConnection::ProcessRequest() {
  //wpi::errs() << "HTTP request: '" << m_request.GetUrl() << "'\n";
  wpi::UrlParser url{m_request.GetUrl(),
                     m_request.GetMethod() == wpi::HTTP_CONNECT};
  if (!url.IsValid()) {
    // failed to parse URL
    SendError(400);
    return;
  }

  wpi::StringRef path;
  if (url.HasPath()) path = url.GetPath();
  //wpi::errs() << "path: \"" << path << "\"\n";

  wpi::StringRef query;
  if (url.HasQuery()) query = url.GetQuery();
  //wpi::errs() << "query: \"" << query << "\"\n";

  const bool isGET = m_request.GetMethod() == wpi::HTTP_GET;
  const bool isPOST = m_request.GetMethod() == wpi::HTTP_POST;
  if ((isGET && path.equals("/") &&
       query.find("action=stream") != wpi::StringRef::npos) ||
      (isGET && path.equals("/stream.mjpg")) ||
      (isPOST && path.equals("/stream"))) {
#if 0
  } else if (isGET && (path.equals("/") || path.equals("/index.html"))) {
    SendStaticResponse(200, "OK", "text/html", GetResource_index_html(), false);
  } else if (isGET && path.equals("/cameraserver.css")) {
    SendStaticResponse(200, "OK", "text/css", GetResource_cameraserver_css(),
                       false);
  } else if (isGET && path.equals("/cameraserver.js")) {
    SendStaticResponse(200, "OK", "text/javascript", GetResource_cameraserver_js(),
                       false);
#endif
  } else if (isGET && path.equals("/bootstrap.min.js")) {
    SendStaticResponse(200, "OK", "text/javascript",
                       wpi::GetResource_bootstrap_4_1_min_js_gz(), true);
  } else if (isGET && path.equals("/coreui.min.css")) {
    SendStaticResponse(200, "OK", "text/css",
                       wpi::GetResource_coreui_2_1_min_css_gz(), true);
  } else if (isGET && path.equals("/coreui.min.js")) {
    SendStaticResponse(200, "OK", "text/javascript",
                       wpi::GetResource_coreui_2_1_min_js_gz(), true);
  } else if (isGET && path.equals("/feather.min.js")) {
    SendStaticResponse(200, "OK", "text/javascript",
                       wpi::GetResource_feather_4_8_min_js_gz(), true);
  } else if (isGET && path.equals("/jquery-3.3.1.slim.min.js")) {
    SendStaticResponse(200, "OK", "text/javascript",
                       wpi::GetResource_jquery_3_3_slim_min_js_gz(), true);
  } else if (isGET && path.equals("/popper.min.js")) {
    SendStaticResponse(200, "OK", "text/javascript",
                       wpi::GetResource_popper_1_14_min_js_gz(), true);
  } else if (isGET && path.equals("/wpilib.png")) {
    SendStaticResponse(200, "OK", "image/png",
                       wpi::GetResource_wpilib_128_png(), false);
  } else {
    SendError(404, "Resource not found");
  }
}

void MyHttpConnection::ProcessWsUpgrade() {
  // Connect the websocket open event to our connected event.
  m_websocket->open.connect_extended([this](auto conn, wpi::StringRef) {
    // wpi::errs() << "websocket connected\n";
    // InitWs(*s);
    conn.disconnect();  // one-shot
  });
  m_websocket->text.connect([this](wpi::StringRef msg, bool) {
    // ProcessWsText(*s, msg);
  });
  m_websocket->binary.connect([this](wpi::ArrayRef<uint8_t> msg, bool) {
    // ProcessWsBinary(*s, msg);
  });
}

ServerImpl::ServerImpl(const ServerConfig& config,
                       wpi::EventLoopRunner& eventLoop, wpi::Logger& logger)
    : m_eventLoop(eventLoop), m_logger(logger), m_config(config) {}

ServerImpl::~ServerImpl() {
  m_eventLoop.ExecSync([=](uv::Loop& loop) {
    m_server->Close();
  });
}

void ServerImpl::Start() {
  m_eventLoop.ExecAsync([this](uv::Loop& loop) {
    m_server = uv::Tcp::Create(loop);

    // bind to listen address and port
    m_server->Bind(m_config.address, m_config.port);

    // when we get a connection, accept it and start reading
    m_server->connection.connect([this] {
      auto stream = m_server->Accept();
      if (!stream) return;
      // wpi::errs() << "Got a connection\n";

      // Close on error
      stream->error.connect([s = stream.get()](uv::Error err) {
        wpi::errs() << "stream error: " << err.str() << '\n';
        s->Close();
      });

      auto conn = std::make_shared<MyHttpConnection>(stream, weak_from_this());
      stream->SetData(conn);
    });

    // start listening for incoming connections
    m_server->Listen();

    wpi::errs() << "CS: Listening on port " << m_config.port << '\n';
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

bool ServerImpl::SetConfigJson(wpi::StringRef config, CS_Status* status) {
  // TODO
  return false;
}

bool ServerImpl::SetConfigJsonObject(const wpi::json& config,
                                     CS_Status* status) {
  // TODO
  return false;
}

wpi::json ServerImpl::GetConfigJsonObject(CS_Status* status) {
  // TODO
  return wpi::json{};
}

void ServerImpl::UpdatePropertyValue(int property, bool setString, int value,
                                     const wpi::Twine& valueStr) {}
