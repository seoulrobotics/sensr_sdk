#pragma once

#include "websocketpp/client.hpp"
#include <functional>
#include <thread>
#include <memory>
namespace sensr {
  class WebSocketEndPointBase {
  public:
    using MsgReceiver = std::function<void(const std::string& msg)>;
    using ErrorReceiver = std::function<void(const std::string& err)>;
    WebSocketEndPointBase() : status_(Status::kConnecting), msg_receiver_(0), err_receiver_(0) {};
    virtual ~WebSocketEndPointBase();

    virtual bool Connect(const std::string &uri, const MsgReceiver& func, const ErrorReceiver& err_func) = 0;
    virtual void Close(websocketpp::close::status::value code) = 0;

  protected:
    enum struct Status {
      kConnecting,
      kOpen,
      kClosed,
      kFailed
    };
    std::thread thread_;
    websocketpp::connection_hdl connection_hdl_;
    Status status_;
    MsgReceiver msg_receiver_;
    ErrorReceiver err_receiver_;
  };
} // namespace sensr