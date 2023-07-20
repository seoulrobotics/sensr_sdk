#pragma once

#include "websocketpp/client.hpp"
#include "../logging.h"
#include <functional>
#include <thread>
#include <memory>

namespace sensr {
  class WebSocketEndPointBase {
  public:
    using MsgReceiver = std::function<void(const std::string& msg)>;
    using ErrorReceiver = std::function<void(const std::string& err)>;

    WebSocketEndPointBase(std::string uri, MsgReceiver msg_cb, ErrorReceiver err_cb);

    virtual ~WebSocketEndPointBase() = default;

    virtual bool Connect() = 0;
    virtual void Close(websocketpp::close::status::value code) = 0;
    bool IsConnected() const { return status_ == Status::kOpen; }

   protected:
    static std::string ConvertToUri(const std::string& protocol, const std::string& address, uint16_t port);

  protected:
    const std::string kUri;

    MsgReceiver msg_receiver_;
    ErrorReceiver err_receiver_;

    enum struct Status {
      kConnecting,
      kOpen,
      kClosed,
      kFailed
    };
    Status status_ = Status::kConnecting;

    websocketpp::connection_hdl connection_hdl_;

    std::thread thread_;

  protected:
    template <typename T>
    void Init(websocketpp::client<T>& endpoint);
    template <typename T>
    void Fin(websocketpp::client<T>& endpoint);
    template <typename T>
    bool Bind(const std::shared_ptr<websocketpp::connection<T>>& con);
    template <typename T>
    void Unbind(websocketpp::client<T>& endpoint, websocketpp::close::status::value code);
    void OnFail(const std::string& err_msg);
    void OnMessage(const std::string& msg, websocketpp::frame::opcode::value opcode);
  };

  template <typename T>
  void WebSocketEndPointBase::Init(websocketpp::client<T>& endpoint) {
    endpoint.clear_access_channels(websocketpp::log::alevel::all);
    endpoint.clear_error_channels(websocketpp::log::elevel::all);

    endpoint.init_asio();
    endpoint.start_perpetual();

    thread_ = std::thread(&websocketpp::client<T>::run, &endpoint);
  }

  template <typename T>
  void WebSocketEndPointBase::Fin(websocketpp::client<T>& endpoint) {
    endpoint.stop_perpetual();
    Close(websocketpp::close::status::going_away);
    if (thread_.joinable()) {
      thread_.join();
    }
  }

  template <typename T>
  bool WebSocketEndPointBase::Bind(const std::shared_ptr<websocketpp::connection<T>>& con) {
    try {
      con->set_open_handler([this] (websocketpp::connection_hdl hdl) {
        status_ = Status::kOpen;
      });
      const std::shared_ptr<websocketpp::connection<T>> connection  = con;
      con->set_close_handler([this, connection] (websocketpp::connection_hdl hdl) {
        auto close_code = connection->get_remote_close_code();
        std::string close_reason = connection->get_remote_close_reason();
        if (close_code == websocketpp::close::status::abnormal_close) {
          OnFail("The connection to SENSR was banned because of no responding.");
        } else if (close_code == websocketpp::close::status::internal_endpoint_error &&
                   close_reason.find("Writing buffer is full") != std::string::npos) {
          OnFail(close_reason);
        } else {
          status_ = Status::kClosed;
        }
      });
    } catch(const std::exception& e) {
      std::string error_msg = "> Failed to connect SENSR.";
      error_msg += e.what();
      ERROR_LOG(error_msg);
      return false;
    }
    return true;
  }

  template <typename T>
  void WebSocketEndPointBase::Unbind(websocketpp::client<T>& endpoint, websocketpp::close::status::value code) {
    std::error_code ec;
    if (connection_hdl_.expired()) {
      // std::cout << "> No connection found" << std::endl;
    } else {
      if (status_ != Status::kClosed) {
        if (auto c = endpoint.get_con_from_hdl(connection_hdl_);
            c->get_state() == websocketpp::session::state::open) {
          endpoint.close(connection_hdl_, code, "", ec);
          if (ec) {
            ERROR_LOG("> Error closing connection : " + ec.message());
          }
        }
      }
    }
    connection_hdl_.reset();
  }
}  // namespace sensr