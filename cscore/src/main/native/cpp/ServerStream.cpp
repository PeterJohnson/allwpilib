/*----------------------------------------------------------------------------*/
/* Copyright (c) 2020 FIRST. All Rights Reserved.                             */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in the root directory of */
/* the project.                                                               */
/*----------------------------------------------------------------------------*/

#include "ServerStream.h"

#include <wpi/ArrayRef.h>
#include <wpi/SmallString.h>
#include <wpi/SmallVector.h>
#include <wpi/WebSocket.h>
#include <wpi/json.h>
#include <wpi/raw_ostream.h>
#include <wpi/uv/Timer.h>
#include <wpi/uv/util.h>

#include "Frame.h"
#include "JpegUtil.h"
#include "Log.h"
#include "ServerImpl.h"
#include "SourceImpl.h"

namespace uv = wpi::uv;

using namespace cs;

namespace {
class MjpegFrameWriteReq : public uv::WriteReq {
 public:
  MjpegFrameWriteReq(Frame frame, Image* image) : m_frame(frame) {
    const char* data = image->data();
    size_t size = image->size();
    size_t locSOF = size;

    // Determine if we need to add DHT to it
    bool addDHT = JpegNeedsDHT(data, &size, &locSOF);

    // header
    double timestamp = frame.GetTime() / 1000000.0;
    wpi::raw_svector_ostream os{m_header};
    os << "\r\n--" << ServerHttpMjpegStream::kBoundary << "\r\n"
       << "Content-Type: image/jpeg\r\n"
       << "Content-Length: " << size << "\r\n"
       << "X-Timestamp: " << timestamp << "\r\n"
       << "\r\n";
    m_bufs.emplace_back(m_header.str());
    if (addDHT) {
      // Insert DHT data immediately before SOF
      m_bufs.emplace_back(data, locSOF);
      m_bufs.emplace_back(JpegGetDHT());
      m_bufs.emplace_back(data + locSOF, image->size() - locSOF);
    } else {
      m_bufs.emplace_back(data, size);
    }
  }

  wpi::ArrayRef<uv::Buffer> GetBufs() const { return m_bufs; }

 private:
  Frame m_frame;
  wpi::SmallString<128> m_header;
  wpi::SmallVector<uv::Buffer, 4> m_bufs;
};
}  // namespace

ServerStream::ServerStream(wpi::uv::Loop& loop, const sockaddr_storage& peer,
                           std::shared_ptr<ServerImpl> server,
                           std::shared_ptr<SourceImpl> source,
                           const Config& config, wpi::Logger& logger)
    : m_peer{peer},
      m_server{server},
      m_source{std::move(source)},
      m_config{config},
      m_logger{logger} {
  if (m_config.fps != 0) m_timePerFrame = 1000000.0 / m_config.fps;
  if (m_averagePeriod < m_timePerFrame) m_averagePeriod = m_timePerFrame * 10;

  server->StartStream(this);
  m_source->EnableSink();
  m_onNewFrameConn = m_source->newFrame.connect_connection([this](Frame frame) {
    if (auto server = m_server.lock()) {
      server->m_newFrameAsync->Send(weak_from_this(), std::move(frame));
    }
  });

  // Start telemetry
  m_activeTelemetry.time = loop.Now().count();
  m_telemetryTimer = uv::Timer::Create(loop);
  m_telemetryTimer->Start(uv::Timer::Time{1000}, uv::Timer::Time{1000});
  m_telemetryTimer->timeout.connect([this] {
    auto now = m_telemetryTimer->GetLoopRef().Now().count();
    m_lastTelemetry = m_activeTelemetry;
    m_lastTelemetry.time = now - m_lastTelemetry.time;
    m_activeTelemetry = Telemetry{};
    m_activeTelemetry.time = now;
  });
  m_telemetryTimer->Unreference();
}

ServerStream::~ServerStream() {
  m_source->DisableSink();
  if (auto server = m_server.lock()) server->StopStream(this);
  m_telemetryTimer->Close();
}

wpi::json ServerStream::GetStreamInfo() const {
  wpi::json j;

  j.emplace("sourceId", m_source->GetName());

  wpi::SmallString<128> ip;
  unsigned int port = 0;
  if (uv::AddrToName(m_peer, &ip, &port) == 0) {
    j.emplace("remoteIp", ip);
    j.emplace("remotePort", port);
  }
  if (m_lastTelemetry.time != 0) {
    double duration = m_lastTelemetry.time / 1000.0;
    j.emplace("actualFps", m_lastTelemetry.frames / duration);
    j.emplace("actualDataRate", m_lastTelemetry.bytes / duration);
  }
  wpi::json config;
  config.emplace("fps", m_config.fps);
  config.emplace("width", m_config.width);
  config.emplace("height", m_config.height);
  config.emplace("quality", m_config.quality);
  j.emplace("config", std::move(config));
  return j;
}

void ServerStream::SendFrame(Frame frame) {
  // don't write another frame until the previous frame finished sending
  if (!m_nextFrameReady) return;

  auto thisFrameTime = frame.GetTime();
  if (thisFrameTime != 0 && m_timePerFrame != 0 && m_lastFrameTime != 0) {
    Frame::Time deltaTime = thisFrameTime - m_lastFrameTime;

    // drop frame if it is early compared to the desired frame rate AND
    // the current average is higher than the desired average
    if (deltaTime < m_timePerFrame && m_averageFrameTime < m_timePerFrame)
      return;

    // update average
    if (m_averageFrameTime != 0) {
      m_averageFrameTime = m_averageFrameTime *
                               (m_averagePeriod - m_timePerFrame) /
                               m_averagePeriod +
                           deltaTime * m_timePerFrame / m_averagePeriod;
    } else {
      m_averageFrameTime = deltaTime;
    }
  }
  m_lastFrameTime = thisFrameTime;

  DEBUG4("sending frame");
  DoSendFrame(std::move(frame));
}

ServerHttpMjpegStream::ServerHttpMjpegStream(wpi::uv::Stream& stream,
                                             const sockaddr_storage& peer,
                                             std::shared_ptr<ServerImpl> server,
                                             std::shared_ptr<SourceImpl> source,
                                             const Config& config,
                                             wpi::Logger& logger)
    : ServerStream{stream.GetLoopRef(), peer, server, source, config, logger},
      m_stream{stream} {
  // Start keepalive
  m_keepAliveTimer = uv::Timer::Create(stream.GetLoopRef());
  m_keepAliveTimer->Start(uv::Timer::Time{200}, uv::Timer::Time{200});
  m_keepAliveTimer->timeout.connect([this] {
    DEBUG4("sending keep-alive");
    m_stream.Write(uv::Buffer("\r\n"), [](auto, auto) {});
  });
  m_keepAliveTimer->Unreference();
}

ServerHttpMjpegStream::~ServerHttpMjpegStream() {
  if (m_keepAliveTimer) m_keepAliveTimer->Close();
}

void ServerHttpMjpegStream::DoSendFrame(Frame frame) {
  int width = frame.GetOriginalWidth();
  int height = frame.GetOriginalHeight();
  Image* image = frame.GetImageMJPEG(width, height, 80);
  if (!image) return;

  // update telemetry
  ++m_activeTelemetry.frames;
  m_activeTelemetry.bytes += image->size();

  // send image
  auto req = std::make_shared<MjpegFrameWriteReq>(std::move(frame), image);
  auto bufs = req->GetBufs();
  m_nextFrameReady = false;
  req->finish.connect([this](auto) { m_nextFrameReady = true; });
  m_stream.Write(bufs, std::move(req));

  // reset keepalive
  m_keepAliveTimer->Again();
}

ServerWebSocketMjpegStream::ServerWebSocketMjpegStream(
    wpi::uv::Loop& loop, wpi::WebSocket& ws, const sockaddr_storage& peer,
    std::shared_ptr<ServerImpl> server, std::shared_ptr<SourceImpl> source,
    const Config& config, wpi::Logger& logger)
    : ServerStream{loop, peer, server, source, config, logger}, m_ws{ws} {}

void ServerWebSocketMjpegStream::DoSendFrame(Frame frame) {
  int width = frame.GetOriginalWidth();
  int height = frame.GetOriginalHeight();
  Image* image = frame.GetImageMJPEG(width, height, 80);
  if (!image) return;

  // update telemetry
  ++m_activeTelemetry.frames;
  m_activeTelemetry.bytes += image->size();

  // send image
  const char* data = image->data();
  size_t size = image->size();
  size_t locSOF = size;

  // Determine if we need to add DHT to it
  bool addDHT = JpegNeedsDHT(data, &size, &locSOF);

  wpi::SmallVector<uv::Buffer, 4> toSend;
  if (addDHT) {
    // Insert DHT data immediately before SOF
    toSend.emplace_back(data, locSOF);
    toSend.emplace_back(JpegGetDHT());
    toSend.emplace_back(data + locSOF, image->size() - locSOF);
  } else {
    toSend.emplace_back(data, size);
  }

  // capture frame so that data pointer remains valid
  m_nextFrameReady = false;
  m_ws.SendBinary(toSend, [this, frame](auto, uv::Error) {
    (void)frame;
    m_nextFrameReady = true;
  });
}
