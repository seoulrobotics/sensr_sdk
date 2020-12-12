#ifndef SENSR_CLIENT_H
#define SENSR_CLIENT_H

#include "sensr_proto/output.pb.h"
#include "sensr_proto/point_cloud.pb.h"
#include "websocket_endpoint.h"
#include <string>
#include <vector>
namespace sensr
{
class Client
{
public:
  using OutputMessageListener = std::function<void(const sensr_proto::OutputMessage& message)>;
  using PointResultListener = std::function<void(const sensr_proto::PointResult& message)>;
  Client(const std::string& address);
  ~Client();
  bool SubscribeMessageListener(const OutputMessageListener& output_listener, const PointResultListener& point_listener = 0);
  void UnsubscribeMessageListener();
private:
  websocket_endpoint output_endpoint_;
  websocket_endpoint point_endpoint_;
  OutputMessageListener output_listener_;
  PointResultListener point_listener_;
  const std::string address_;

  void OnResultMessage(const std::string& msg);
  void OnPointMessage(const std::string& msg);
};
} // namespace sensr

#endif // SENSR_CLIENT_H