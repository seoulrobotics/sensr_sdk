#pragma once

#include <string>

#include "sensr_message_listener.h"
#include "websocket/websocket_endpoint_base.h"

namespace sensr {
class MessageBrokerBase {
 public:
  MessageBrokerBase(const std::string& address,
                    uint16_t port,
                    const std::string& cert_path = std::string());
  virtual ~MessageBrokerBase() = default;

  bool IsConnected() const;
  bool IsListening() const;

  void Reconnect();

  void TryAttachListener(const std::shared_ptr<MessageListener>& listener);
  void TryDetachListener(const std::shared_ptr<MessageListener>& listener);

  void ClearListeners();

 private:
  virtual bool IsAttachable(const MessageListener& listener) const = 0;

 protected:
  std::unique_ptr<WebSocketEndPointBase> endpoint_;

  std::vector<std::shared_ptr<MessageListener>> listeners_;
};

class OutputMessageBroker : public MessageBrokerBase {
 public:
  OutputMessageBroker(const std::string& address,
                      uint16_t port,
                      const std::string& cert_path = std::string());

 private:
  bool IsAttachable(const MessageListener& listener) const override;

  void OnMessage(const std::string& message);
  void OnError(const std::string& error);
};

class PointMessageBroker : public MessageBrokerBase {
 public:
  PointMessageBroker(const std::string& address,
                     uint16_t port,
                     const std::string& cert_path = std::string());

 private:
  bool IsAttachable(const MessageListener& listener) const override;

  void OnMessage(const std::string& message);
  void OnError(const std::string& error);
};
}  // namespace sensr
