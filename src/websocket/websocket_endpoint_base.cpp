#include "websocket_endpoint_base.h"
#include <memory>
namespace sensr {
  WebSocketEndPointBase::WebSocketEndPointBase()
  : status_(Status::kConnecting), msg_receiver_(0), err_receiver_(0) {}
  
  void WebSocketEndPointBase::OnOpen(websocketpp::connection_hdl hdl) {
    status_ = Status::kOpen;
  }
  void WebSocketEndPointBase::OnClose(websocketpp::connection_hdl hdl) {
    status_ = Status::kClosed;
  }
} // namespace sensr