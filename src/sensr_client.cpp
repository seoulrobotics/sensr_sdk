#include "sensr_client.h"
#include "websocket/websocket_endpoint.h"
#include "websocket/websocket_secure_endpoint.h"
#include "sensr_message_listener.h"
#include <algorithm>

namespace sensr
{
  Client::Client(const std::string& address, const std::string& cert_path) 
      : address_(address), use_ssl_(!cert_path.empty()), is_reconnecting_(false) {
    if (use_ssl_) {
      output_endpoint_.reset(new WebSocketSecureEndPoint(cert_path));
      point_endpoint_.reset(new WebSocketSecureEndPoint(cert_path));
    } else {
      output_endpoint_.reset(new WebSocketEndPoint());
      point_endpoint_.reset(new WebSocketEndPoint());
    }    
  }

  Client::~Client()
  {
    for (const auto& listener : listeners_) {
      UnsubscribeMessageListener(listener);
    }
    StopReconnecting();
  }

  void Client::Reconnect() {
    if (!is_reconnecting_) {
      StopReconnecting();
      reconnection_thread_ = std::thread([this]() {
        is_reconnecting_ = true;
        auto temp = listeners_;
        size_t reconnection_attempts_ = 0;
        while (reconnection_attempts_++ < Client::kMaxReconnectTrialCount) {
          INFO_LOG("Reconnecting...");
          for (const auto& listener : listeners_) {
            UnsubscribeMessageListener(listener);
          }
          for (const auto& listener : temp) {
            SubscribeMessageListener(listener);
          }
          bool is_output_ep_connected = true;
          bool is_point_ep_connected = true;
          if (IsResultListening()) {
            if (!output_endpoint_->IsConnected()) {
              is_output_ep_connected = false;
            }
          }
          if (IsPointListening()) {
            if (!point_endpoint_->IsConnected()) {
              is_point_ep_connected = false;
            }
          }

          if (is_output_ep_connected && is_point_ep_connected) {
            INFO_LOG("Reconnected!");
            break;
          } else if (!is_reconnecting_) {
            INFO_LOG("Stop reconnection!");
            break;
          }
          std::this_thread::sleep_for(std::chrono::seconds(1));
        }
        is_reconnecting_ = false;
      });
    }
  }

  bool Client::SubscribeMessageListener(const std::shared_ptr<MessageListener>& listener) { 
    bool ret = false;
    // Connect to SENSR
    if (std::find(listeners_.begin(), listeners_.end(), listener) == listeners_.end()) {
      // OutputMessage Port
      if (listener->IsOutputMessageListening()) {
         if (!IsResultListening()) {
          ret = output_endpoint_->Connect(GetProtocol() + "://" + address_ + ":5050", 
          std::bind(&Client::OnResultMessage, this, std::placeholders::_1),
          std::bind(&Client::OnResultError, this, std::placeholders::_1));
        } else {
          ret = true;
        }
      } 
      // PointResult Port
      if (listener->IsPointResultListening()) {
        if (!IsPointListening()) {
          ret = point_endpoint_->Connect(GetProtocol() + "://" + address_ + ":5051", 
          std::bind(&Client::OnPointMessage, this, std::placeholders::_1),
          std::bind(&Client::OnPointError, this, std::placeholders::_1));
        } else {
          ret = true;
        }
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
      output_endpoint_->Close(websocketpp::close::status::normal);
    }
    // Close if no listener is listening PointResult.
    if (std::none_of(listeners_.begin(), listeners_.end(), [](const std::shared_ptr<MessageListener>& listener) {
      return listener->IsPointResultListening();
    })) {
      point_endpoint_->Close(websocketpp::close::status::normal);
    }
  }

  void Client::OnResultMessage(const std::string& message) {
    sensr_proto::OutputMessage output;
    if (!output.ParseFromString(message)) {
      std::cerr << "Wrong format" << std::endl;
      return;
    }
    bool is_output_buffer_overflow = false;
    if (output.has_event() && output.event().has_health() && 
        output.event().health().master() == sensr_proto::SystemHealth_Status_OUTPUT_BUFFER_OVERFLOW) {
      is_output_buffer_overflow = true;
    }
    for (const auto& listener : listeners_) {
      if (listener->IsOutputMessageListening()) {
        listener->OnGetOutputMessage(output);
        if (is_output_buffer_overflow) {
          listener->OnError(MessageListener::Error::kOutputBufferOverflow, message);
        }  
      }
    }
  }

  bool Client::IsResultListening() const {
    return std::any_of(listeners_.begin(), listeners_.end(), [](const std::shared_ptr<MessageListener>& listener) {
      return listener->IsOutputMessageListening();
    });
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

  void Client::OnResultError(const std::string &err) {
    for (const auto& listener : listeners_) {
      if (listener->IsOutputMessageListening()) {
        listener->OnError(MessageListener::Error::kOutputMessageConnection, err);
      }
    }
  }

  void Client::OnPointError(const std::string &err) {
    for (const auto& listener : listeners_) {
      if (listener->IsPointResultListening()) {
        listener->OnError(MessageListener::Error::kPointResultConnection, err);
      }
    }
  }

  bool Client::IsPointListening() const {
    return std::any_of(listeners_.begin(), listeners_.end(), [](const std::shared_ptr<MessageListener>& listener) {
      return listener->IsPointResultListening();
    });
  }

  void Client::StopReconnecting() {
    if (reconnection_thread_.joinable()) {
      is_reconnecting_ = false;
      reconnection_thread_.join();
    }
  }

} // namespace sensr
