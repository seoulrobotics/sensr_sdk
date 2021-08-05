#include "websocket_endpoint_base.h"
#include <memory>
namespace sensr {
  void WebSocketEndPointBase::OnOpen(websocketpp::connection_hdl hdl) {
    status_ = Status::kOpen;
  }
  void WebSocketEndPointBase::OnClose(websocketpp::connection_hdl hdl) {
    status_ = Status::kClosed;
  }
} // namespace sensr