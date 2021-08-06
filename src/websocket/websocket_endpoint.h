#pragma once

#include "./websocket_endpoint_base.h"
#include "websocketpp/config/asio_no_tls_client.hpp"
#include "websocketpp/extensions/permessage_deflate/enabled.hpp"
#include <functional>
#include <thread>
#include <memory>
namespace sensr {
  class WebSocketEndPoint : public WebSocketEndPointBase {
  public:
    using websocketpp_client = websocketpp::client<websocketpp::config::asio_client>;
    WebSocketEndPoint();
    ~WebSocketEndPoint() override;

    bool Connect(const std::string &uri, 
                 const WebSocketEndPointBase::MsgReceiver& func, 
                 const WebSocketEndPointBase::ErrorReceiver& err_func) override;
    void Close(websocketpp::close::status::value code) override;
  
  private:
    websocketpp_client endpoint_;
  };
} // namespace sensr