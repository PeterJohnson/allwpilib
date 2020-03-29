/*----------------------------------------------------------------------------*/
/* Copyright (c) 2020 FIRST. All Rights Reserved.                             */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in the root directory of */
/* the project.                                                               */
/*----------------------------------------------------------------------------*/

#ifndef CSCORE_SERVERSTREAM_H_
#define CSCORE_SERVERSTREAM_H_

#include <stdint.h>

#include <memory>

#include <wpi/uv/Stream.h>

#include "Frame.h"

namespace wpi {
class Logger;
class WebSocket;
class json;
namespace uv {
class Loop;
class Timer;
}  // namespace uv
}  // namespace wpi

namespace cs {

class ServerImpl;
class SourceImpl;

class ServerStream : public std::enable_shared_from_this<ServerStream> {
 public:
  struct Config {
    int width = 0;
    int height = 0;
    int fps = 0;
    int quality = 0;
  };

  ServerStream(wpi::uv::Loop& loop, const sockaddr_storage& peer,
               std::shared_ptr<ServerImpl> server,
               std::shared_ptr<SourceImpl> source, const Config& config,
               wpi::Logger& logger);
  ~ServerStream();

  void SendFrame(Frame frame);

  wpi::json GetStreamInfo() const;

 protected:
  virtual void DoSendFrame(Frame frame) = 0;

  sockaddr_storage m_peer;
  std::weak_ptr<ServerImpl> m_server;
  std::shared_ptr<SourceImpl> m_source;

  wpi::sig::ScopedConnection m_onNewFrameConn;

  std::shared_ptr<wpi::uv::Timer> m_telemetryTimer;

  bool m_nextFrameReady = true;
  Frame::Time m_lastSourceTime = 0;
  Frame::Time m_lastFrameTime = 0;
  Frame::Time m_timePerFrame = 0;
  Frame::Time m_averageFrameTime = 0;
  Frame::Time m_averagePeriod = 1000000;  // 1 second window
  Config m_config;

  struct Telemetry {
    unsigned int frames = 0;
    unsigned int bytes = 0;
    uint64_t time = 0;
  };
  Telemetry m_activeTelemetry;
  Telemetry m_lastTelemetry;

  wpi::Logger& m_logger;
};

class ServerHttpMjpegStream : public ServerStream {
 public:
  // The boundary used for the M-JPEG stream.
  // It separates the multipart stream of pictures
  static constexpr const char* kBoundary = "boundarydonotcross";

  ServerHttpMjpegStream(wpi::uv::Stream& stream, const sockaddr_storage& peer,
                        std::shared_ptr<ServerImpl> server,
                        std::shared_ptr<SourceImpl> source,
                        const Config& config, wpi::Logger& logger);
  ~ServerHttpMjpegStream();

 private:
  void DoSendFrame(Frame frame) override;

  std::shared_ptr<wpi::uv::Timer> m_keepAliveTimer;
  wpi::uv::Stream& m_stream;
};

class ServerWebSocketMjpegStream : public ServerStream {
 public:
  ServerWebSocketMjpegStream(wpi::uv::Loop& loop, wpi::WebSocket& ws,
                             const sockaddr_storage& peer,
                             std::shared_ptr<ServerImpl> server,
                             std::shared_ptr<SourceImpl> source,
                             const Config& config, wpi::Logger& logger);

 private:
  void DoSendFrame(Frame frame) override;

  wpi::WebSocket& m_ws;
};

}  // namespace cs

#endif  // CSCORE_SERVERSTREAM_H_
