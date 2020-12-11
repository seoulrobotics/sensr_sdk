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
  Client() = default;
  ~Client();
  //bool Connect(const std::string& address);
  //void Disconnect();
  bool SubscribeMessageListener(const std::string& address, OutputMessageListener listener);
  bool SubscribeMessageListener(const std::string& address, PointResultListener listener);
  void UnsubscribeMessageListener(const std::string& address, OutputMessageListener listener);
  void UnsubscribeMessageListener(const std::string& address, PointResultListener listener);
  //bool ReceiveMessage(OutputMessage& output_result);
  //bool ReceiveMessage(PointResultListener& output_result);
private:
  websocket_endpoint<sensr_proto::OutputMessage> output_endpoint_;
  websocket_endpoint<sensr_proto::PointResult> point_endpoint_;
  std::vector<OutputMessageListener> output_listener_;
  std::vector<PointResultListener> point_listener_;

  typedef websocketpp::client<websocketpp::config::asio_client> client;
  void OnResultMessage(const sensr_proto::OutputMessage& msg);
  void OnPointMessage(const sensr_proto::PointResult& msg);

  // std::function does not provide operator==, getAddress() replace this.)
  template<typename T, typename... U>
  size_t getAddress(std::function<T(U...)> f) {
    typedef T(fnType)(U...);
    fnType ** fnPointer = f.template target<fnType*>();
    return (size_t) *fnPointer;
  }
};
} // namespace sensr

#endif // SENSR_CLIENT_H