#pragma once

#include "websocketpp/client.hpp"
#include "websocketpp/config/asio_client.hpp"
#include "websocketpp/extensions/permessage_deflate/enabled.hpp"
#include <functional>
#include <thread>
#include <memory>
namespace sensr {
  using websocketpp_client = websocketpp::client<websocketpp::config::asio_client>;
  class WebSocketEndPoint {
  public:
    using MsgReceiver = std::function<void(const std::string& msg)>;
    using ErrorReceiver = std::function<void(const std::string& err)>;
    WebSocketEndPoint();
    ~WebSocketEndPoint();

    bool Connect(const std::string &uri, MsgReceiver func, ErrorReceiver err_func);
    void Close(websocketpp::close::status::value code);

  private:
    void OnOpen(websocketpp_client *c, websocketpp::connection_hdl hdl);
    void OnFail(websocketpp_client *c, websocketpp::connection_hdl hdl);
    void OnClose(websocketpp_client *c, websocketpp::connection_hdl hdl);
    void OnMessage(websocketpp::connection_hdl hdl, websocketpp_client::message_ptr msg);
    bool OnVerifyCertificate(bool preverified, boost::asio::ssl::verify_context& ctx);
    static bool VerifySubjectAlternativeName(const char * hostname, X509 * cert);
    static bool VerifyCommonName(char const * hostname, X509 * cert);
    enum struct Status {
      kConnecting,
      kOpen,
      kClosed,
      kFailed
    };
    websocketpp_client endpoint_;
    std::thread thread_;
    websocketpp::connection_hdl connection_hdl_;
    Status status_;
    MsgReceiver msg_receiver_;
    ErrorReceiver err_receiver_;
  };
} // namespace sensr