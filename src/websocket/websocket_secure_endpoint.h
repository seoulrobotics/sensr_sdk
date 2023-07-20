#pragma once

#include "./websocket_endpoint_base.h"
#include "websocketpp/config/asio_client.hpp"
#include "websocketpp/extensions/permessage_deflate/enabled.hpp"

namespace sensr {
class WebSocketSecureEndPoint : public WebSocketEndPointBase {
  using websocketpp_client = websocketpp::client<websocketpp::config::asio_tls_client>;
  using context_ptr = websocketpp::lib::shared_ptr<websocketpp::lib::asio::ssl::context>;

 public:
  WebSocketSecureEndPoint(const std::string& address,
                          uint16_t port,
                          WebSocketEndPointBase::MsgReceiver msg_cb,
                          WebSocketEndPointBase::ErrorReceiver err_cb,
                          const std::string& cert_path);
  ~WebSocketSecureEndPoint() override;

  bool Connect() override;
  void Close(websocketpp::close::status::value code) override;

 private:
  context_ptr OnTlsInit(websocketpp::connection_hdl hdl);

 private:
  static constexpr const char* kProtocol = "wss";

  const std::string cert_path_;

  websocketpp_client endpoint_;
};
}  // namespace sensr