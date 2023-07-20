#include "message_broker.h"

#include "websocket/websocket_endpoint.h"
#include "websocket/websocket_secure_endpoint.h"

namespace sensr {

MessageBrokerBase::MessageBrokerBase(const std::string& address,
                                     uint16_t port,
                                     const std::string& cert_path) {
  if (cert_path.empty()) {
    endpoint_ = std::make_unique<WebSocketEndPoint>(address, port);
  } else {
    endpoint_ = std::make_unique<WebSocketSecureEndPoint>(address, port, cert_path);
  }
}

bool MessageBrokerBase::IsConnected() const { return endpoint_->IsConnected(); }

bool MessageBrokerBase::IsListening() const { return !listeners_.empty(); }

void MessageBrokerBase::TryAttachListener(const std::shared_ptr<MessageListener>& listener) {
  bool start_connection = listeners_.empty();

  if (listener != nullptr && IsAttachable(*listener)) {
    if (auto it = std::find(listeners_.cbegin(), listeners_.cend(), listener);
        it == listeners_.cend()) {
      listeners_.push_back(listener);
    }
  }

  if (start_connection && !listeners_.empty()) {
    endpoint_->Connect();
  }
}

void MessageBrokerBase::TryDetachListener(const std::shared_ptr<MessageListener>& listener) {
  bool end_connection = !listeners_.empty();

  if (listener != nullptr && IsAttachable(*listener)) {
    if (auto it = std::find(listeners_.cbegin(), listeners_.cend(), listener);
        it != listeners_.cend()) {
      listeners_.erase(it);
    }
  }

  if (end_connection && listeners_.empty()) {
    endpoint_->Close(websocketpp::close::status::normal);
  }
}

void MessageBrokerBase::ClearListeners() {
  listeners_.clear();
  endpoint_->Close(websocketpp::close::status::normal);
}

OutputMessageBroker::OutputMessageBroker(const std::string& address,
                                         uint16_t port,
                                         const std::string& cert_path)
    : MessageBrokerBase(address, port, cert_path) {
  endpoint_->SetCallbacks([this](auto&& msg) { OnMessage(msg); },
                          [this](auto&& err) { OnError(err); });
}

bool OutputMessageBroker::IsAttachable(const MessageListener& listener) const {
  return listener.IsOutputMessageListening();
}

void OutputMessageBroker::OnMessage(const std::string& message) {
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
    listener->OnGetOutputMessage(output);
    if (is_output_buffer_overflow) {
      listener->OnError(MessageListener::Error::kOutputBufferOverflow, message);
    }
  }
}

void OutputMessageBroker::OnError(const std::string& error) {
  for (const auto& listener : listeners_) {
    listener->OnError(MessageListener::Error::kOutputMessageConnection, error);
  }
}

PointMessageBroker::PointMessageBroker(const std::string& address,
                                       uint16_t port,
                                       const std::string& cert_path)
    : MessageBrokerBase(address, port, cert_path) {
  endpoint_->SetCallbacks([this](auto&& msg) { OnMessage(msg); },
                          [this](auto&& err) { OnError(err); });
}

bool PointMessageBroker::IsAttachable(const MessageListener& listener) const {
  return listener.IsPointResultListening();
}

void PointMessageBroker::OnMessage(const std::string& message) {
  sensr_proto::PointResult output;
  if (!output.ParseFromString(message)) {
    std::cerr << "Wrong format" << std::endl;
    return;
  }
  for (const auto& listener : listeners_) {
    listener->OnGetPointResult(output);
  }
}

void PointMessageBroker::OnError(const std::string& error) {
  for (const auto& listener : listeners_) {
    listener->OnError(MessageListener::Error::kPointResultConnection, error);
  }
}
}  // namespace sensr
