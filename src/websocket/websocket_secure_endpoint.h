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
                 const ErrorReceiver& err_func) override;
    void Close(websocketpp::close::status::value code) override;

  private:
    context_ptr OnTLSInit(websocketpp::connection_hdl hdl);
    void OnFail(websocketpp_client *c, websocketpp::connection_hdl hdl);
    void OnMessage(websocketpp::connection_hdl hdl, websocketpp_client::message_ptr msg);
    bool OnVerifyCertificate(bool preverified, boost::asio::ssl::verify_context& ctx);
    static bool VerifySubjectAlternativeName(const char * hostname, X509 * cert);
    static bool VerifyCommonName(char const * hostname, X509 * cert);
    websocketpp_client endpoint_;
    const std::string cert_path_;
    std::array<const std::string, 1> certified_names_;
  };
} // namespace sensr