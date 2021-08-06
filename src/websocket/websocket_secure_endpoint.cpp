#include "./websocket_secure_endpoint.h"
#include "../logging.h"

namespace sensr
{   
  WebSocketSecureEndPoint::WebSocketSecureEndPoint(const std::string& cert_path) : 
    WebSocketEndPointBase(),
    cert_path_(cert_path)
  {
    Init(endpoint_);
  }

  WebSocketSecureEndPoint::~WebSocketSecureEndPoint() {
    Fin(endpoint_);
  }

  bool WebSocketSecureEndPoint::Connect(const std::string &uri,
                 const WebSocketEndPointBase::MsgReceiver& func,
                 const WebSocketEndPointBase::ErrorReceiver& err_func) {
    
    if (!connection_hdl_.expired()) {
      INFO_LOG(uri + " is already connected.");
      return true;
    }
    bool ret = false;
    try {
      if (!cert_path_.empty()) { 
        endpoint_.set_tls_init_handler(std::bind(
          &WebSocketSecureEndPoint::OnTLSInit,
          this,
          std::placeholders::_1));
      } else {
        ERROR_LOG("> Certificate file path is empty.");
        return false;
      }
      std::error_code ec;
      // Register our message handler
      auto con = endpoint_.get_connection(uri, ec);
      if (ec) {
        ERROR_LOG("> WSS Connect initialization error: " + ec.message());
        return false;
      }
      
      connection_hdl_ = con->get_handle();

      con->set_message_handler([this](websocketpp::connection_hdl hdl, websocketpp_client::message_ptr msg) {
        OnMessage(msg->get_payload(), msg->get_opcode());
      });
      con->set_fail_handler([this](websocketpp::connection_hdl hdl) {
        websocketpp_client::connection_ptr con = endpoint_.get_con_from_hdl(hdl);
        OnFail(con->get_ec().message());
      });
      ret = Bind(con, func, err_func);
      endpoint_.connect(con);
    } catch(const std::exception& e) {
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

  WebSocketSecureEndPoint::context_ptr WebSocketSecureEndPoint::OnTLSInit(websocketpp::connection_hdl hdl) {
    context_ptr ctx = websocketpp::lib::make_shared<boost::asio::ssl::context>(boost::asio::ssl::context::sslv23);

    try {
        ctx->set_options(boost::asio::ssl::context::default_workarounds |
                         boost::asio::ssl::context::sslv23);
        ctx->set_verify_mode(boost::asio::ssl::verify_peer);
        ctx->load_verify_file(cert_path_);
    } catch (std::exception& e) {
        std::cout << "Error at TLS setup: " << e.what() << std::endl;
        ctx = nullptr;
    }
    return ctx;
  } 
} // namespace sensr