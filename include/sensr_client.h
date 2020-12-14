#ifndef SENSR_CLIENT_H
#define SENSR_CLIENT_H

#include "sensr_proto/output.pb.h"
#include "sensr_proto/point_cloud.pb.h"
#include <string>
#include <memory>
#include <functional>

namespace sensr
{
  class MessageListener;
  class websocket_endpoint;
  class Client
  {
  public:
    Client(const std::string &address);
    ~Client();
    bool Reconnect(); // Call when Error occur
    bool SubscribeMessageListener(const std::shared_ptr<MessageListener>& listener);
    void UnsubscribeMessageListener(const std::shared_ptr<MessageListener>& listener);

  private:
    std::unique_ptr<websocket_endpoint> output_endpoint_;
    std::unique_ptr<websocket_endpoint> point_endpoint_;
    std::vector<std::shared_ptr<MessageListener>> listeners_;
    const std::string address_;

    void OnResultMessage(const std::string &msg);
    void OnPointMessage(const std::string &msg);
    void OnResultError(const std::string &err);
    void OnPointError(const std::string &err);
  };
} // namespace sensr

#endif // SENSR_CLIENT_H