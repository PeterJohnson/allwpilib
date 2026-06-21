// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

#include <string>

#include "CommandTestBase.hpp"
#include "wpi/commands2/Commands.hpp"
#include "wpi/nt/BooleanTopic.hpp"
#include "wpi/nt/NetworkTableInstance.hpp"
#include "wpi/smartdashboard/SmartDashboard.hpp"

using namespace wpi::cmd;

class CommandSendableButtonTest : public CommandTestBase {
 protected:
  CommandSendableButtonTest() {
    m_schedule = 0;
    m_cancel = 0;
    m_command = StartEnd([this] { m_schedule++; }, [this] { m_cancel++; });
    m_key = "command" + std::to_string(m_nextKey++);
    m_publish = wpi::nt::NetworkTableInstance::GetDefault()
                    .GetBooleanTopic("/SmartDashboard/" + m_key + "/running")
                    .Publish();
    m_publish.Set(false);
    wpi::SmartDashboard::PutData(m_key, m_command->get());
    wpi::SmartDashboard::UpdateValues();
  }

  void SetRunning(bool running) {
    m_publish.Set(running);
    wpi::SmartDashboard::UpdateValues();
  }

  int m_schedule;
  int m_cancel;
  std::string m_key;
  wpi::nt::BooleanPublisher m_publish;
  std::optional<CommandPtr> m_command;

 private:
  inline static int m_nextKey = 0;
};

TEST_CASE_METHOD(CommandSendableButtonTest,
                 "CommandSendableButtonTest trueAndNotScheduledSchedules",
                 "[commandsv2][command]") {
  // Not scheduled and true -> scheduled
  GetScheduler().Run();
  wpi::SmartDashboard::UpdateValues();
  CHECK_FALSE(m_command->IsScheduled());
  CHECK(0 == m_schedule);
  CHECK(0 == m_cancel);

  SetRunning(true);
  GetScheduler().Run();
  CHECK(m_command->IsScheduled());
  CHECK(1 == m_schedule);
  CHECK(0 == m_cancel);
}

TEST_CASE_METHOD(CommandSendableButtonTest,
                 "CommandSendableButtonTest trueAndScheduledNoOp",
                 "[commandsv2][command]") {
  // Scheduled and true -> no-op
  wpi::cmd::CommandScheduler::GetInstance().Schedule(m_command.value());
  GetScheduler().Run();
  wpi::SmartDashboard::UpdateValues();
  CHECK(m_command->IsScheduled());
  CHECK(1 == m_schedule);
  CHECK(0 == m_cancel);

  SetRunning(true);
  GetScheduler().Run();
  CHECK(m_command->IsScheduled());
  CHECK(1 == m_schedule);
  CHECK(0 == m_cancel);
}

TEST_CASE_METHOD(CommandSendableButtonTest,
                 "CommandSendableButtonTest falseAndNotScheduledNoOp",
                 "[commandsv2][command]") {
  // Not scheduled and false -> no-op
  GetScheduler().Run();
  wpi::SmartDashboard::UpdateValues();
  CHECK_FALSE(m_command->IsScheduled());
  CHECK(0 == m_schedule);
  CHECK(0 == m_cancel);

  SetRunning(false);
  GetScheduler().Run();
  CHECK_FALSE(m_command->IsScheduled());
  CHECK(0 == m_schedule);
  CHECK(0 == m_cancel);
}

TEST_CASE_METHOD(CommandSendableButtonTest,
                 "CommandSendableButtonTest falseAndScheduledCancel",
                 "[commandsv2][command]") {
  // Scheduled and false -> cancel
  wpi::cmd::CommandScheduler::GetInstance().Schedule(m_command.value());
  GetScheduler().Run();
  wpi::SmartDashboard::UpdateValues();
  CHECK(m_command->IsScheduled());
  CHECK(1 == m_schedule);
  CHECK(0 == m_cancel);

  SetRunning(false);
  GetScheduler().Run();
  CHECK_FALSE(m_command->IsScheduled());
  CHECK(1 == m_schedule);
  CHECK(1 == m_cancel);
}
