#include "sensr_client.h"

const int MAX_ADDRESS_LEN = 512;

namespace sensr
{
  Client::~Client()
  {
    output_listener_.clear();
    output_listener_.clear();
    output_endpoint_.close(websocketpp::close::status::going_away);
    point_endpoint_.close(websocketpp::close::status::going_away);
  }

  /*bool Client::ReceiveMessage(OutputMessage &message)
  {
    zmq::message_t network_message;
    if (subscriber_.recv(&network_message, 0))
    {
      std::string msg_string(network_message.data<char>(), network_message.size());
      message.ParseFromString(msg_string);
      return true;
    }
    return false;
  }

  bool Client::Connect(const std::string& address) {
    if (output_endpoint_.connect("ws://" + address + ":5050") == -1 ||
        point_endpoint_.connect("ws://" + address + ":5051") == -1) {
          return false
    }
    return true;
  }

  void Client::Disconnect() {
    int close_code = websocketpp::close::status::normal;
    std::string reason;
    output_endpoint_.close(id, close_code);
    point_endpoint_.close(id, close_code);
  }*/


  bool Client::SubscribeMessageListener(const std::string& address, OutputMessageListener listener) {    
    if (std::find_if(output_listener_.begin(), output_listener_.end(), [this, &listener](const OutputMessageListener& cb) {
      return getAddress(listener) == getAddress(cb);
    }) == output_listener_.end()) {
      output_listener_.push_back(listener);
      return output_endpoint_.connect("ws://" + address + ":5050", [this](const sensr_proto::OutputMessage& message) {
        for (const auto& listener : output_listener_) {
          listener(message);
        }
      }) != -1 ? true : false;
    }
    return true;
  }

  void Client::UnsubscribeMessageListener(const std::string& address, OutputMessageListener listener) {
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

  bool Client::SubscribeMessageListener(const std::string& address, PointResultListener listener) {
    if (std::find_if(point_listener_.begin(), point_listener_.end(), [this, &listener](const PointResultListener& cb) {
      return getAddress(listener) == getAddress(cb);
    }) == point_listener_.end()) {
      point_listener_.push_back(listener);
      return point_endpoint_.connect("ws://" + address + ":5051", [this](const sensr_proto::PointResult& message) {
        for (const auto& listener : point_listener_) {
          listener(message);
        }
      }) != -1 ? true : false;
    }
    return true;
  }

  void Client::UnsubscribeMessageListener(const std::string& address, PointResultListener listener) {
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

  void Client::OnResultMessage(const sensr_proto::OutputMessage& message) {
    for (const auto& listener : output_listener_) {
      listener(message);
    }
  }

  void Client::OnPointMessage(const sensr_proto::PointResult& message) {
    for (const auto& listener : point_listener_) {
      listener(message);
    }
  }

} // namespace sensr
