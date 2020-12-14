#include "sensr_client.h"
#include "websocket_endpoint.h"
#include "sensr_message_listener.h"
#include <algorithm>

namespace sensr
{
  Client::Client(const std::string& address) : address_(address) {
    output_endpoint_.reset(new websocket_endpoint());
    point_endpoint_.reset(new websocket_endpoint());
  }

  Client::~Client()
  {
    for (const auto& listener : listeners_) {
      UnsubscribeMessageListener(listener);
    }
  }

  bool Client::SubscribeMessageListener(const std::shared_ptr<MessageListener>& listener) { 
    bool ret = false;
    // Connect to SENSR
    if (std::find(listeners_.begin(), listeners_.end(), listener) == listeners_.end()) {
      // OutputMessage Port
      if (listener->IsOutputMessageListening()) {
        ret = output_endpoint_->connect("ws://" + address_ + ":5050", std::bind(&Client::OnResultMessage, this, std::placeholders::_1)); 
      } 
      // PointResult Port
      if (listener->IsPointResultListening()) {
        ret = point_endpoint_->connect("ws://" + address_ + ":5051", std::bind(&Client::OnPointMessage, this, std::placeholders::_1)); 
      }
    }
    // Add listener in case connection success.
    if (ret) {    
      listeners_.push_back(listener);
    }
    return ret;
  }

  void Client::UnsubscribeMessageListener(const std::shared_ptr<MessageListener>& listener) {
    auto itr = std::find(listeners_.begin(), listeners_.end(), listener);
    if (itr != listeners_.end()) {      
      listeners_.erase(itr);
    }
    // Close if no listener is listening OutputMessage.
    if (std::none_of(listeners_.begin(), listeners_.end(), [](const std::shared_ptr<MessageListener>& listener) {
      return listener->IsOutputMessageListening();
    })) {
      output_endpoint_->close(websocketpp::close::status::normal);
    }
    // Close if no listener is listening PointResult.
    if (std::none_of(listeners_.begin(), listeners_.end(), [](const std::shared_ptr<MessageListener>& listener) {
      return listener->IsPointResultListening();
    })) {
      point_endpoint_->close(websocketpp::close::status::normal);
    }
  }

  void Client::OnResultMessage(const std::string& message) {
    sensr_proto::OutputMessage output;
    if (!output.ParseFromString(message)) {
      std::cerr << "Wrong format" << std::endl;
      return;
    }
    for (const auto& listener : listeners_) {
      if (listener->IsOutputMessageListening()) {
        listener->OnGetOutpuMessage(output);
      }
    }
  }

  void Client::OnPointMessage(const std::string& message) {
    sensr_proto::PointResult output;
    if (!output.ParseFromString(message)) {
      std::cerr << "Wrong format" << std::endl;
      return;
    }
    for (const auto& listener : listeners_) {
      if (listener->IsPointResultListening()) {
        listener->OnGetPointResult(output);
      }
    }
  }

} // namespace sensr
