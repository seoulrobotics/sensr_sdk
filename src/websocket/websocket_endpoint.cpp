#include "./websocket_endpoint.h"
#include "../logging.h"

namespace sensr
{   
  WebSocketEndPoint::WebSocketEndPoint() : 
    WebSocketEndPointBase()
  {
    Init(endpoint_);
    // endpoint_.clear_access_channels(websocketpp::log::alevel::all);
    // endpoint_.clear_error_channels(websocketpp::log::elevel::all);

    // endpoint_.init_asio();
    // endpoint_.start_perpetual();

    // thread_ = std::thread(&websocketpp_client::run, &endpoint_);
  }

  WebSocketEndPoint::~WebSocketEndPoint() {
    Fin(endpoint_);
    // endpoint_.stop_perpetual();
    // Close(websocketpp::close::status::going_away);
    // if (thread_.joinable()) {
    //   thread_.join();
    // }
  }

  bool WebSocketEndPoint::Connect(const std::string &uri, 
                 const WebSocketEndPointBase::MsgReceiver& func, 
                 const WebSocketEndPointBase::ErrorReceiver& err_func) {
    
    if (!connection_hdl_.expired()) {
      INFO_LOG(uri + " is already connected.");
      return true;
    }
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
    return Bind(endpoint_, uri, func, err_func);
    // try {
    //   std::error_code ec;
    //   // Register our message handler
    //   websocketpp_client::connection_ptr con = endpoint_.get_connection(uri, ec);
    //   if (ec) {
    //     ERROR_LOG("> Connect initialization error: " + ec.message());
    //     return false;
    //   }
    //   msg_receiver_ = func;
    //   err_receiver_ = err_func; 
    //   connection_hdl_ = con->get_handle();
    //   con->set_open_handler(std::bind(
    //     &WebSocketEndPoint::OnOpen,
    //     this,
    //     &endpoint_,
    //     std::placeholders::_1));
    //   con->set_fail_handler(std::bind(
    //     &WebSocketEndPoint::OnFail,
    //     this,
    //     &endpoint_,
    //     std::placeholders::_1));
    //   con->set_message_handler(std::bind(
    //     &WebSocketEndPoint::OnMessage,
    //     this,
    //     std::placeholders::_1,
    //     std::placeholders::_2));

    //   endpoint_.connect(con);
    // } catch(const std::exception& e) {
    //   std::string error_msg = "> Failed to connect SENSR.";
    //   error_msg += e.what();
    //   ERROR_LOG(error_msg);
    //   return false;
    // }
    // return true;
  };

  void WebSocketEndPoint::Close(websocketpp::close::status::value code) {
    Disconnect(endpoint_, code);
    // std::error_code ec;
    // if (connection_hdl_.expired()) {
    //   //std::cout << "> No connection found" << std::endl;
    // } else {
    //   if (status_ != Status::kClosed) {
    //     // Only close open connections
    //     endpoint_.close(connection_hdl_, code, "", ec);
    //     if (ec) {
    //       ERROR_LOG("> Error closing connection : " + ec.message());
    //     }
    //   }
    // }    
    // connection_hdl_.reset();
    // msg_receiver_ = 0;
    // err_receiver_ = 0;
  }  

  // void WebSocketEndPoint::OnOpen(websocketpp_client *c, websocketpp::connection_hdl hdl) {
  //   status_ = Status::kOpen;
  // }

  // void WebSocketEndPoint::OnFail(websocketpp_client *c, websocketpp::connection_hdl hdl) {
  //   status_ = Status::kFailed;
  //   if (err_receiver_ != 0) {
  //     websocketpp_client::connection_ptr con = c->get_con_from_hdl(hdl);
  //     err_receiver_(con->get_ec().message());
  //   }
  // }

  // void WebSocketEndPoint::OnClose(websocketpp_client *c, websocketpp::connection_hdl hdl) {
  //   status_ = Status::kClosed;
  // }

  void WebSocketEndPoint::OnMessage(websocketpp::connection_hdl hdl, websocketpp_client::message_ptr msg) {
    if (status_ == Status::kOpen && msg->get_opcode() == websocketpp::frame::opcode::BINARY) {
      if (msg_receiver_ != 0) {
        msg_receiver_(msg->get_payload());
      }
    }     
  }  
} // namespace sensr