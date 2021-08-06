#pragma once

#include "./websocket_endpoint_base.h"
#include "websocketpp/config/asio_client.hpp"
#include "websocketpp/extensions/permessage_deflate/enabled.hpp"
#include <functional>
#include <thread>
#include <memory>
namespace sensr {
  class WebSocketSecureEndPoint : public WebSocketEndPointBase {
  public:
    using websocketpp_client = websocketpp::client<websocketpp::config::asio_tls_client>;
    using context_ptr = websocketpp::lib::shared_ptr<websocketpp::lib::asio::ssl::context>;
    WebSocketSecureEndPoint(const std::string& cert_path);
    ~WebSocketSecureEndPoint() override;

    bool Connect(const std::string &uri,
                 const WebSocketEndPointBase::MsgReceiver& func,
                 const WebSocketEndPointBase::ErrorReceiver& err_func) override;
    void Close(websocketpp::close::status::value code) override;

  private:
    context_ptr OnTlsInit(websocketpp::connection_hdl hdl);
    websocketpp_client endpoint_;
    const std::string cert_path_;
  };
} // namespace sensr