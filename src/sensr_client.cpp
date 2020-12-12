#include "sensr_client.h"
#include "websocket_endpoint.h"

namespace sensr
{
  Client::Client(const std::string& address) : address_(address) {
    output_endpoint_.reset(new websocket_endpoint());
    point_endpoint_.reset(new websocket_endpoint());
  }

  Client::~Client()
  {
    if (output_listener_ != 0) {
      output_endpoint_->close(websocketpp::close::status::going_away);
    }
    if (point_listener_ != 0) {
      point_endpoint_->close(websocketpp::close::status::going_away);
    }
  }

  bool Client::SubscribeMessageListener(const OutputMessageListener& output_listener, const PointResultListener& point_listener) { 
    output_listener_ = output_listener;      
    bool ret1 = output_endpoint_->connect("ws://" + address_ + ":5050", std::bind(&Client::OnResultMessage, this, std::placeholders::_1));
    bool ret2 = true;
    if (point_listener != 0) {
      point_listener_ = point_listener;
      ret2 = point_endpoint_->connect("ws://" + address_ + ":5051", std::bind(&Client::OnPointMessage, this, std::placeholders::_1));
    }
    return ret1 && ret2;
  }

  void Client::UnsubscribeMessageListener() {
    if (output_listener_ != 0) {
      output_listener_ = 0;
      output_endpoint_->close(websocketpp::close::status::normal);
    }
    if (point_listener_ != 0) {
      point_listener_ = 0;
      point_endpoint_->close(websocketpp::close::status::normal);
    }
  }

  void Client::OnResultMessage(const std::string& message) {
    if (output_listener_ != 0) {
      sensr_proto::OutputMessage output;
      if (output.ParseFromString(message)) {
        output_listener_(output);
      } else {
        std::cerr << "Wrong format" << std::endl;
      }
    }
  }

  void Client::OnPointMessage(const std::string& message) {
    if (point_listener_ != 0) {
      sensr_proto::PointResult result;
      if (result.ParseFromString(message)) {
        point_listener_(result);
      } else {
        std::cerr << "Wrong format" << std::endl;
      }
    }
  }

} // namespace sensr
