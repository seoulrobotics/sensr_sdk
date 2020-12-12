#include "sensr_client.h"

const int MAX_ADDRESS_LEN = 512;

namespace sensr
{
  Client::~Client()
  {
    output_listener_.clear();
    point_listener_.clear();
    output_endpoint_.close(websocketpp::close::status::going_away);
    point_endpoint_.close(websocketpp::close::status::going_away);
  }

  bool Client::SubscribeMessageListener(const std::string& address, const OutputMessageListener& listener) {    
    if (std::find_if(output_listener_.begin(), output_listener_.end(), [this, &listener](const OutputMessageListener& cb) {
      return getAddress(listener) == getAddress(cb);
    }) == output_listener_.end()) {
      output_listener_.push_back(listener);
      return output_endpoint_.connect("ws://" + address + ":5050", std::bind(&Client::OnResultMessage, this, std::placeholders::_1))!= -1 ? true : false;
      
      // return output_endpoint_.connect("ws://" + address + ":5050", [this](const sensr_proto::OutputMessage& message) {
      //   for (const auto& listener : output_listener_) {
      //     listener(message);
      //   }
      // }) != -1 ? true : false;
    }
    return true;
  }

  void Client::UnsubscribeMessageListener(const std::string& address, const OutputMessageListener& listener) {
    auto itr = std::find_if(output_listener_.begin(), output_listener_.end(), [this, &listener](const OutputMessageListener& cb) {
      return getAddress(listener) == getAddress(cb);
    });
    if (itr != output_listener_.end()) {
      output_listener_.erase(itr);
    }
    if (output_listener_.empty()) {
      output_endpoint_.close(websocketpp::close::status::normal);
    }
  }

  bool Client::SubscribeMessageListener(const std::string& address, const PointResultListener& listener) {
    if (std::find_if(point_listener_.begin(), point_listener_.end(), [this, &listener](const PointResultListener& cb) {
      return getAddress(listener) == getAddress(cb);
    }) == point_listener_.end()) {
      point_listener_.push_back(listener);
      return point_endpoint_.connect("ws://" + address + ":5051", std::bind(&Client::OnPointMessage, this, std::placeholders::_1)) != -1 ? true : false;
      
      // return point_endpoint_.connect("ws://" + address + ":5051", [this](const sensr_proto::PointResult& message) {
      //   for (const auto& listener : point_listener_) {
      //     listener(message);
      //   }
      // }) != -1 ? true : false;
    }
    return true;
  }

  void Client::UnsubscribeMessageListener(const std::string& address, const PointResultListener& listener) {
    auto itr = std::find_if(point_listener_.begin(), point_listener_.end(), [this, &listener](const PointResultListener& cb) {
      return getAddress(listener) == getAddress(cb);
    });
    if (itr != point_listener_.end()) {
      point_listener_.erase(itr);
    }
    if (point_listener_.empty()) {
      point_endpoint_.close(websocketpp::close::status::normal);
    }
  }

  void Client::OnResultMessage(const std::string& message) {
    sensr_proto::OutputMessage output;
    output.ParseFromString(message); 
    for (const auto& listener : output_listener_) {
      listener(output);
    }
  }

  void Client::OnPointMessage(const std::string& message) {
    sensr_proto::PointResult result;
    result.ParseFromString(message); 
    for (const auto& listener : point_listener_) {
      listener(result);
    }
  }

} // namespace sensr
