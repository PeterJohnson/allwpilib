/*----------------------------------------------------------------------------*/
/* Copyright (c) 2020 FIRST. All Rights Reserved.                             */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in the root directory of */
/* the project.                                                               */
/*----------------------------------------------------------------------------*/

#ifndef CSCORE_SERVERIMPL_H_
#define CSCORE_SERVERIMPL_H_

#include <memory>
#include <vector>

#include <wpi/Signal.h>
#include <wpi/StringRef.h>
#include <wpi/Twine.h>
#include <wpi/uv/Async.h>
#include <wpi/uv/Tcp.h>

#include "Frame.h"
#include "PropertyContainer.h"
#include "cscore_cpp.h"

namespace wpi {
class EventLoopRunner;
class Logger;
}  // namespace wpi

namespace cs {

class ServerStream;

class ServerImpl : public PropertyContainer,
                   public std::enable_shared_from_this<ServerImpl> {
 public:
  ServerImpl(const ServerConfig& config, wpi::EventLoopRunner& eventLoop,
             wpi::Logger& logger);
  ~ServerImpl();
  ServerImpl(const ServerImpl&) = delete;
  ServerImpl& operator=(const ServerImpl&) = delete;

  void Start();

  const ServerConfig& GetConfig() const { return m_config; }

  static ServerConfig ParseConfig(wpi::StringRef config);
  static ServerConfig ParseConfig(const wpi::json& config);

  bool SetConfigJson(const wpi::json& config, CS_Status* status) override;
  wpi::json GetConfigJson(CS_Status* status) const override;
  wpi::json GetInfoJson(CS_Status* status) const override;

  wpi::json GetStreamsInfoJson() const;

  wpi::sig::Signal<> streamsChanged;

 protected:
  // PropertyContainer implementation
  void UpdatePropertyValue(int property, bool setString, int value,
                           const wpi::Twine& valueStr) override;

 private:
  class Connection;
  friend class Connection;
  friend class ServerStream;

  void StartStream(ServerStream* conn);
  void StopStream(ServerStream* conn);

  wpi::EventLoopRunner& m_eventLoop;
  wpi::Logger& m_logger;
  ServerConfig m_config;
  std::shared_ptr<wpi::uv::Tcp> m_server;
  using NewFrameAsync = wpi::uv::Async<std::weak_ptr<ServerStream>, Frame>;
  std::shared_ptr<NewFrameAsync> m_newFrameAsync;
  std::vector<ServerStream*> m_streams;
};

}  // namespace cs

#endif  // CSCORE_SERVERIMPL_H_
