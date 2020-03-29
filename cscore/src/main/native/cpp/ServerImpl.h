/*----------------------------------------------------------------------------*/
/* Copyright (c) 2020 FIRST. All Rights Reserved.                             */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in the root directory of */
/* the project.                                                               */
/*----------------------------------------------------------------------------*/

#ifndef CSCORE_SERVERIMPL_H_
#define CSCORE_SERVERIMPL_H_

#include <wpi/uv/Tcp.h>

#include "PropertyContainer.h"
#include "cscore_cpp.h"

namespace wpi {
class EventLoopRunner;
class Logger;
}  // namespace wpi

namespace cs {

class ServerImpl : public PropertyContainer {
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

  bool SetConfigJson(wpi::StringRef config, CS_Status* status) override;
  bool SetConfigJsonObject(const wpi::json& config, CS_Status* status) override;
  wpi::json GetConfigJsonObject(CS_Status* status) override;

 protected:
  // PropertyContainer implementation
  void UpdatePropertyValue(int property, bool setString, int value,
                           const wpi::Twine& valueStr) override;

 private:
  wpi::EventLoopRunner& m_eventLoop;
  wpi::Logger& m_logger;
  ServerConfig m_config;
  std::shared_ptr<wpi::uv::Tcp> m_server;
};

}  // namespace cs

#endif  // CSCORE_SERVERIMPL_H_
