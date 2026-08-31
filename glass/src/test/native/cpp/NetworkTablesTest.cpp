// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

#include "wpi/glass/networktables/NetworkTables.hpp"

#include <stdint.h>

#include <cstddef>
#include <span>
#include <string_view>
#include <utility>
#include <vector>

#include <catch2/catch_test_macros.hpp>

#include "wpi/nt/NetworkTableInstance.hpp"
#include "wpi/nt/ntcore_cpp.hpp"
#include "wpi/util/MessagePack.hpp"
#include "wpi/util/mpack.h"
#include "wpi/util/raw_ostream.hpp"

namespace {
class MsgpackWriter : public mpack::mpack_writer_t {
 public:
  MsgpackWriter() {
    mpack::mpack_writer_init(this, m_buf, sizeof(m_buf));
    mpack::mpack_writer_set_context(this, &m_os);
    mpack::mpack_writer_set_flush(this, [](mpack::mpack_writer_t* writer,
                                           const char* buffer, size_t count) {
      static_cast<wpi::util::raw_ostream*>(writer->context)
          ->write(buffer, count);
    });
  }

  std::vector<uint8_t> Finish() {
    REQUIRE(mpack::mpack_writer_destroy(this) == mpack::mpack_ok);
    return std::move(m_bytes);
  }

 private:
  std::vector<uint8_t> m_bytes;
  wpi::util::raw_uvector_ostream m_os{m_bytes};
  char m_buf[128];
};

std::vector<uint8_t> MakeClientsMetadata(std::string_view clientId) {
  MsgpackWriter writer;
  mpack::mpack_start_array(&writer, 1);
  mpack::mpack_start_map(&writer, 3);
  mpack::mpack_write_str(&writer, "id");
  mpack::mpack_write_str(&writer, clientId);
  mpack::mpack_write_str(&writer, "conn");
  mpack::mpack_write_str(&writer, "unit-test");
  mpack::mpack_write_str(&writer, "ver");
  mpack::mpack_write_u16(&writer, 4);
  mpack::mpack_finish_map(&writer);
  mpack::mpack_finish_array(&writer);
  return writer.Finish();
}

std::vector<uint8_t> MakeSubscribersMetadata(
    std::span<const std::string_view> topics, bool prefixMatch) {
  MsgpackWriter writer;
  mpack::mpack_start_array(&writer, 1);
  mpack::mpack_start_map(&writer, 3);
  mpack::mpack_write_str(&writer, "uid");
  mpack::mpack_write_int(&writer, 1);
  mpack::mpack_write_str(&writer, "topics");
  mpack::mpack_start_array(&writer, static_cast<uint32_t>(topics.size()));
  for (auto topic : topics) {
    mpack::mpack_write_str(&writer, topic);
  }
  mpack::mpack_finish_array(&writer);
  mpack::mpack_write_str(&writer, "options");
  mpack::mpack_start_map(&writer, prefixMatch ? 1u : 0u);
  if (prefixMatch) {
    mpack::mpack_write_str(&writer, "prefix");
    mpack::mpack_write_bool(&writer, true);
  }
  mpack::mpack_finish_map(&writer);
  mpack::mpack_finish_map(&writer);
  mpack::mpack_finish_array(&writer);
  return writer.Finish();
}

std::vector<uint8_t> MakeEmptySubscribersMetadata() {
  MsgpackWriter writer;
  mpack::mpack_start_array(&writer, 0);
  mpack::mpack_finish_array(&writer);
  return writer.Finish();
}

void PublishMsgpack(wpi::nt::NetworkTableInstance inst, std::string_view name,
                    const std::vector<uint8_t>& value) {
  auto publisher = wpi::nt::Publish(wpi::nt::GetTopic(inst.GetHandle(), name),
                                    NT_RAW, "msgpack");
  REQUIRE(publisher != 0);
  REQUIRE(wpi::nt::SetRaw(publisher, value));
}

bool TreeContainsEntry(
    const std::vector<wpi::glass::NetworkTablesModel::TreeNode>& nodes,
    std::string_view name) {
  for (auto&& node : nodes) {
    if (node.entry && node.entry->info.name == name) {
      return true;
    }
    if (TreeContainsEntry(node.children, name)) {
      return true;
    }
  }
  return false;
}
}  // namespace

TEST_CASE("NetworkTablesModel ShowsExactSubscriberOnlyTopics",
          "[networktables]") {
  auto inst = wpi::nt::NetworkTableInstance::Create();
  {
    wpi::glass::NetworkTablesModel model{inst};

    PublishMsgpack(inst, "$clients", MakeClientsMetadata("client"));
    constexpr std::string_view topic = "/NeedsValue";
    PublishMsgpack(inst, "$clientsub$client",
                   MakeSubscribersMetadata(
                       std::span<const std::string_view>{&topic, 1}, false));

    model.Update();

    auto* entry = model.GetEntry(topic);
    REQUIRE(entry);
    CHECK(entry->info.type == NT_UNASSIGNED);
    CHECK_FALSE(entry->value);
    CHECK(entry->subscriberOnly);
    CHECK(TreeContainsEntry(model.GetTreeRoot(), topic));

    PublishMsgpack(inst, "$clientsub$client", MakeEmptySubscribersMetadata());

    model.Update();

    CHECK(model.GetEntry(topic) == nullptr);
    CHECK_FALSE(TreeContainsEntry(model.GetTreeRoot(), topic));
  }
  wpi::nt::NetworkTableInstance::Destroy(inst);
}

TEST_CASE("NetworkTablesModel KeepsPublishedTopicsWithoutValues",
          "[networktables]") {
  auto inst = wpi::nt::NetworkTableInstance::Create();
  {
    wpi::glass::NetworkTablesModel model{inst};

    PublishMsgpack(inst, "$clients", MakeClientsMetadata("client"));
    constexpr std::string_view topic = "/NeedsPublisherValue";
    PublishMsgpack(inst, "$clientsub$client",
                   MakeSubscribersMetadata(
                       std::span<const std::string_view>{&topic, 1}, false));

    model.Update();

    auto* entry = model.GetEntry(topic);
    REQUIRE(entry);
    CHECK(entry->subscriberOnly);

    auto publisher = wpi::nt::Publish(
        wpi::nt::GetTopic(inst.GetHandle(), topic), NT_DOUBLE, "double");
    REQUIRE(publisher != 0);

    model.Update();

    entry = model.GetEntry(topic);
    REQUIRE(entry);
    CHECK(entry->info.type == NT_DOUBLE);
    CHECK_FALSE(entry->value);
    CHECK_FALSE(entry->subscriberOnly);
    CHECK(TreeContainsEntry(model.GetTreeRoot(), topic));

    PublishMsgpack(inst, "$clientsub$client", MakeEmptySubscribersMetadata());

    model.Update();

    entry = model.GetEntry(topic);
    REQUIRE(entry);
    CHECK(entry->info.type == NT_DOUBLE);
    CHECK_FALSE(entry->value);
    CHECK_FALSE(entry->subscriberOnly);
    CHECK(TreeContainsEntry(model.GetTreeRoot(), topic));

    wpi::nt::Unpublish(publisher);

    model.Update();

    CHECK(model.GetEntry(topic) == nullptr);
    CHECK_FALSE(TreeContainsEntry(model.GetTreeRoot(), topic));
  }
  wpi::nt::NetworkTableInstance::Destroy(inst);
}

TEST_CASE("NetworkTablesModel IgnoresPrefixSubscriberOnlyTopics",
          "[networktables]") {
  auto inst = wpi::nt::NetworkTableInstance::Create();
  {
    wpi::glass::NetworkTablesModel model{inst};

    PublishMsgpack(inst, "$clients", MakeClientsMetadata("client"));
    constexpr std::string_view prefix = "/Prefix";
    PublishMsgpack(inst, "$clientsub$client",
                   MakeSubscribersMetadata(
                       std::span<const std::string_view>{&prefix, 1}, true));

    model.Update();

    CHECK(model.GetEntry(prefix) == nullptr);
    CHECK_FALSE(TreeContainsEntry(model.GetTreeRoot(), prefix));
  }
  wpi::nt::NetworkTableInstance::Destroy(inst);
}
