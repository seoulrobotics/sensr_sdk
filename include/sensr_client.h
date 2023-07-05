#pragma once

#include "sensr_proto/output.pb.h"
#include "sensr_proto/point_cloud.pb.h"
#include <string>
#include <memory>
#include <functional>
#include <atomic>
#include <thread>

namespace sensr
{
  class MessageListener;
  class WebSocketEndPointBase;
  class Client
  {
  public:
    Client(const std::string &address, const std::string& cert_path = "");
    ~Client();
    void Reconnect(); // Call when Error occur
    bool SubscribeMessageListener(const std::shared_ptr<MessageListener>& listener);
    void UnsubscribeMessageListener(const std::shared_ptr<MessageListener>& listener);

  private:
    std::unique_ptr<WebSocketEndPointBase> output_endpoint_;
    std::unique_ptr<WebSocketEndPointBase> point_endpoint_;
    std::vector<std::shared_ptr<MessageListener>> listeners_;
    const std::string address_;
    bool use_ssl_;
    std::atomic<bool> is_reconnecting_;
    std::thread reconnection_thread_;

    void OnResultMessage(const std::string &msg);
    void OnPointMessage(const std::string &msg);
    void OnResultError(const std::string &err);
    void OnPointError(const std::string &err);
    bool IsResultListening() const;
    bool IsPointListening() const;
    void StopReconnecting();
    std::string GetProtocol() const { return use_ssl_ ? "wss" : "ws"; }
  };
} // namespace sensr
