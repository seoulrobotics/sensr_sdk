#include "./websocket_endpoint.h"
#include "../logging.h"

namespace sensr
{   
  WebSocketEndPoint::WebSocketEndPoint() : 
    WebSocketEndPointBase()
  {
    Init(endpoint_);
  }

  WebSocketEndPoint::~WebSocketEndPoint() {
    Fin(endpoint_);
  }

  bool WebSocketEndPoint::Connect(const std::string &uri, 
                 const WebSocketEndPointBase::MsgReceiver& func, 
                 const WebSocketEndPointBase::ErrorReceiver& err_func) {
    if (!connection_hdl_.expired()) {
      INFO_LOG(uri + " is already connected.");
      return true;
    }
    bool ret = false;
    try {
      std::error_code ec;
      // Register our message handler
      auto con = endpoint_.get_connection(uri, ec);
      if (ec) {
        ERROR_LOG("> Connect initialization error: " + ec.message());
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
  }

  void WebSocketEndPoint::Close(websocketpp::close::status::value code) {
    Unbind(endpoint_, code);
  }
} // namespace sensr