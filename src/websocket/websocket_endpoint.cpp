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
    try {
      std::error_code ec;
      // Register our message handler
      auto con = endpoint_.get_connection(uri, ec);
      if (ec) {
        ERROR_LOG("> Connect initialization error: " + ec.message());
        return false;
      }

      connection_hdl_ = con->get_handle();

      con->set_message_handler(std::bind(
        &WebSocketEndPoint::OnMessage,
        this,
        std::placeholders::_1,
        std::placeholders::_2));
      con->set_fail_handler(std::bind(
          &WebSocketEndPoint::OnFail,
          this,
          &endpoint_,
          std::placeholders::_1));
    } catch(const std::exception& e) {
      std::string error_msg = "> Failed to connect SENSR.";
      error_msg += e.what();
      ERROR_LOG(error_msg);
      return false;
    }
    return Bind(endpoint_, uri, func, err_func);
  }

  void WebSocketEndPoint::Close(websocketpp::close::status::value code) {
    Unbind(endpoint_, code);
  }  

  void WebSocketEndPoint::OnMessage(websocketpp::connection_hdl hdl, websocketpp_client::message_ptr msg) {
    if (status_ == Status::kOpen && msg->get_opcode() == websocketpp::frame::opcode::BINARY) {
      if (msg_receiver_ != 0) {
        msg_receiver_(msg->get_payload());
      }
    }     
  }

  void WebSocketEndPoint::OnFail(websocketpp_client *c, websocketpp::connection_hdl hdl) {
    status_ = Status::kFailed;
    if (err_receiver_ != 0) {
      websocketpp_client::connection_ptr con = c->get_con_from_hdl(hdl);
      err_receiver_(con->get_ec().message());
    }
  }
} // namespace sensr