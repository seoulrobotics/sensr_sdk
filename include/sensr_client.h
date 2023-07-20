#pragma once

#include <atomic>
#include <memory>
#include <string>
#include <thread>

#include "sensr_proto/output.pb.h"
#include "sensr_proto/point_cloud.pb.h"

namespace sensr {
class MessageListener;
class MessageBrokerBase;

class Client {
 public:
  Client(const std::string& address, const std::string& cert_path = "");
  ~Client();
  void Reconnect();  // Call when Error occur
  bool SubscribeMessageListener(const std::shared_ptr<MessageListener>& listener);
  void UnsubscribeMessageListener(const std::shared_ptr<MessageListener>& listener);

 private:
  void ClearListeners();

 private:
  enum struct MessageType : uint32_t { Output = 0u, Points };
  std::array<std::unique_ptr<MessageBrokerBase>, 2u> message_brokers_;

  std::atomic<bool> is_reconnecting_ = false;
  std::thread reconnection_thread_;
};
}  // namespace sensr
