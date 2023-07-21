#include "websocket_endpoint_base.h"

namespace sensr {
WebSocketEndPointBase::WebSocketEndPointBase(const std::string& protocol,
                                             const std::string& address,
                                             uint16_t port)
    : kUri(ConvertToUri(protocol, address, port)) {}

void WebSocketEndPointBase::SetCallbacks(MsgReceiver msg_cb, ErrorReceiver err_cb) {
  msg_receiver_ = std::move(msg_cb);
  err_receiver_ = std::move(err_cb);
}

std::string WebSocketEndPointBase::ConvertToUri(const std::string& protocol,
                                                const std::string& address,
                                                uint16_t port) {
  std::stringstream ss;
  ss << protocol << "://" << address << ":" << port;
  return ss.str();
}

void WebSocketEndPointBase::OnFail(const std::string& err_msg) {
  status_ = Status::kFailed;
  if (err_receiver_) {
    err_receiver_(err_msg);
  }
}

void WebSocketEndPointBase::OnMessage(const std::string& msg,
                                      websocketpp::frame::opcode::value opcode) {
  if (status_ == Status::kOpen && opcode == websocketpp::frame::opcode::BINARY) {
    if (msg_receiver_) {
      msg_receiver_(msg);
    }
  }
}
}  // namespace sensr