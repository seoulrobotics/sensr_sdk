#include "websocket_endpoint_base.h"
#include <memory>
namespace sensr {
  WebSocketEndPointBase::WebSocketEndPointBase()
  : status_(Status::kConnecting), msg_receiver_(), err_receiver_() {}
  
  void WebSocketEndPointBase::OnFail(const std::string& err_msg) {
    status_ = Status::kFailed;
    if (err_receiver_) {
      err_receiver_(err_msg);
    }
  }

  void WebSocketEndPointBase::OnMessage(const std::string& msg, websocketpp::frame::opcode::value opcode) {
    if (status_ == Status::kOpen && opcode == websocketpp::frame::opcode::BINARY) {
      if (msg_receiver_) {
        msg_receiver_(msg);
      }
    }     
  } 
} // namespace sensr