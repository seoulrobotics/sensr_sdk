#include "sensr_client.h"

#include <algorithm>

#include "message_broker.h"
#include "sensr_message_listener.h"
#include "websocket/websocket_endpoint.h"
#include "websocket/websocket_secure_endpoint.h"

namespace sensr {
Client::Client(const std::string& address, const std::string& cert_path) {
  const uint16_t kOutputPort = 5050u;
  const uint16_t kPointPort = 5051u;

  message_brokers_[static_cast<uint32_t>(MessageType::Output)] =
      std::make_unique<OutputMessageBroker>(address, kOutputPort, cert_path);
  message_brokers_[static_cast<uint32_t>(MessageType::Points)] =
      std::make_unique<PointMessageBroker>(address, kPointPort, cert_path);
}

Client::~Client() {
  ClearListeners();

  is_reconnecting_.store(false);

  if (reconnection_thread_.joinable()) {
    reconnection_thread_.join();
  }
}

void Client::Reconnect() {
  if (is_reconnecting_) {
    return;
  }

  if (reconnection_thread_.joinable()) {
    reconnection_thread_.join();
  }

  is_reconnecting_ = true;

  reconnection_thread_ = std::thread([this] {
    size_t reconnection_counter = 0u;
    while (is_reconnecting_) {
      INFO_LOG("Reconnecting... TryCount: "s + std::to_string(++reconnection_counter));

      for (const auto& broker : message_brokers_) {
        broker->Reconnect();
      }

      if (std::all_of(message_brokers_.begin(), message_brokers_.end(), [](auto&& broker) {
            return !broker->IsListening() || broker->IsConnected();
          })) {
        INFO_LOG("Reconnected!");
        is_reconnecting_ = false;
      } else {
        using namespace std::chrono_literals;
        std::this_thread::sleep_for(100ms);
      }
    }
  });
}

bool Client::SubscribeMessageListener(const std::shared_ptr<MessageListener>& listener) {
  bool ret = true;
  for (const auto& broker : message_brokers_) {
    broker->TryAttachListener(listener);
    ret = ret && (!broker->IsListening() || broker->IsConnected());
  }
  return ret;
}

void Client::ClearListeners() {
  for (const auto& broker : message_brokers_) {
    broker->ClearListeners();
  }
}

void Client::UnsubscribeMessageListener(const std::shared_ptr<MessageListener>& listener) {
  for (const auto& broker : message_brokers_) {
    broker->TryDetachListener(listener);
  }
}
}  // namespace sensr
