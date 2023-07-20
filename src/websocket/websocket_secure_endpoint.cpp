#include "./websocket_secure_endpoint.h"

#include <exception>

#include "../logging.h"

namespace sensr {
WebSocketSecureEndPoint::WebSocketSecureEndPoint(const std::string& address,
                                                 uint16_t port,
                                                 WebSocketEndPointBase::MsgReceiver msg_cb,
                                                 WebSocketEndPointBase::ErrorReceiver err_cb,
                                                 const std::string& cert_path)
    : WebSocketEndPointBase(WebSocketEndPointBase::ConvertToUri(kProtocol, address, port),
                            std::move(msg_cb),
                            std::move(err_cb)),
      cert_path_(cert_path) {
  Init(endpoint_);
}

WebSocketSecureEndPoint::~WebSocketSecureEndPoint() { Fin(endpoint_); }

bool WebSocketSecureEndPoint::Connect() {
  if (!connection_hdl_.expired()) {
    INFO_LOG(uri + " is already connected.");
    return true;
  }
  bool ret = false;
  try {
    if (!cert_path_.empty()) {
      endpoint_.set_tls_init_handler(
          std::bind(&WebSocketSecureEndPoint::OnTlsInit, this, std::placeholders::_1));
    } else {
      throw std::invalid_argument("> Certificate file path is empty.");
    }
    std::error_code ec;
    // Register our message handler
    auto con = endpoint_.get_connection(kUri, ec);
    if (ec) {
      ERROR_LOG("> WSS Connect initialization error: " + ec.message());
      return false;
    }

    connection_hdl_ = con->get_handle();

    con->set_message_handler(
        [this](websocketpp::connection_hdl hdl, websocketpp_client::message_ptr msg) {
          OnMessage(msg->get_payload(), msg->get_opcode());
        });
    con->set_fail_handler([this](websocketpp::connection_hdl hdl) {
      websocketpp_client::connection_ptr con = endpoint_.get_con_from_hdl(hdl);
      OnFail(con->get_ec().message());
    });
    ret = Bind(con);
    endpoint_.connect(con);
  } catch (const std::exception& e) {
    std::string error_msg = "> Failed to connect SENSR.";
    error_msg += e.what();
    ERROR_LOG(error_msg);
    ret = false;
  }

  return ret;
};

void WebSocketSecureEndPoint::Close(websocketpp::close::status::value code) {
  Unbind(endpoint_, code);
}

WebSocketSecureEndPoint::context_ptr WebSocketSecureEndPoint::OnTlsInit(
    websocketpp::connection_hdl hdl) {
  const auto ssl_method = websocketpp::lib::asio::ssl::context::sslv23;
  context_ptr ctx = websocketpp::lib::make_shared<websocketpp::lib::asio::ssl::context>(ssl_method);

  try {
    ctx->set_options(websocketpp::lib::asio::ssl::context::default_workarounds | ssl_method);
    ctx->set_verify_mode(websocketpp::lib::asio::ssl::verify_peer);
    ctx->load_verify_file(cert_path_);
  } catch (std::exception& e) {
    ERROR_LOG(std::string("Error at TLS setup: ") + e.what());
    ctx = nullptr;
  }
  return ctx;
}
}  // namespace sensr