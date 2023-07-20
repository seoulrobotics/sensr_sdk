#pragma once

#include "./websocket_endpoint_base.h"
#include "websocketpp/config/asio_no_tls_client.hpp"
#include "websocketpp/extensions/permessage_deflate/enabled.hpp"

namespace sensr {
class WebSocketEndPoint : public WebSocketEndPointBase {
  using websocketpp_client = websocketpp::client<websocketpp::config::asio_client>;

 public:
  WebSocketEndPoint(const std::string& address,
                    uint16_t port,
                    WebSocketEndPointBase::MsgReceiver msg_cb,
                    WebSocketEndPointBase::ErrorReceiver err_cb);
  ~WebSocketEndPoint() override;

  bool Connect() override;
  void Close(websocketpp::close::status::value code) override;

 private:
  static constexpr const char* kProtocol = "ws";

  websocketpp_client endpoint_;
};
}  // namespace sensr