#include "sensr_client.h"

#include <algorithm>

#include "message_broker.h"
#include "sensr_message_listener.h"
#include "websocket/websocket_endpoint.h"
#include "websocket/websocket_secure_endpoint.h"

namespace sensr {
Client::Client(const std::string& address, const std::string& cert_path) : is_reconnecting_(false) {
  const uint16_t kOutputPort = 5050u;
  const uint16_t kPointPort = 5051u;

  message_brokers_[static_cast<uint32_t>(MessageType::Output)] =
      std::make_unique<OutputMessageBroker>(address, kOutputPort, cert_path);
  message_brokers_[static_cast<uint32_t>(MessageType::Points)] =
      std::make_unique<PointMessageBroker>(address, kPointPort, cert_path);
}

Client::~Client() {
  ClearListeners();

  io_context_.stop();
  if (reconnection_thread_.joinable()) {
    reconnection_thread_.join();
  }
}

void Client::Reconnect() {
  if (!is_reconnecting_) {
    if (reconnection_thread_.joinable()) {
      reconnection_thread_.join();
    }

    is_reconnecting_ = true;
    io_context_.restart();

    asio::post(io_context_, [this]() { reconnection_async(); });
    reconnection_thread_ = std::thread([this] { io_context_.run(); });
  }
}

void Client::reconnection_async() {
  INFO_LOG("Reconnecting...");
  // auto temp = listeners_;

  // ClearListeners();

  // for (const auto& listener : temp) {
  //   SubscribeMessageListener(listener);
  // }
  // const bool is_output_connected = !IsResultListening() || output_endpoint_->IsConnected();
  // const bool is_point_connected = !IsPointListening() || point_endpoint_->IsConnected();

  if (false) {
    INFO_LOG("Reconnected!");
    is_reconnecting_ = false;
  } else {
    asio::steady_timer sleep_timer(io_context_, asio::chrono::seconds(1));
    sleep_timer.wait();
    reconnection_async();
  }
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
