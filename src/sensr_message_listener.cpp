#include "sensr_message_listener.h"
#include "logging.h"
#include <iostream>

namespace sensr {
    MessageListener::MessageListener(ListeningType listening_type) 
    : listening_type_(listening_type) {

    }

    void MessageListener::OnError(MessageListener::Error error, const std::string& reason) {
        switch (error) {
            case Error::kOutputMessageConnection: 
            case Error::kPointResultConnection:
            case Error::kOutputBufferOverflow: {
                ERROR_LOG("Lost SENSR Connection fail(Reason: " + reason + "). Please reconnect."); 
                break;
            }
            default:
                break;
        }
    }

    void MessageListener::OnGetOutpuMessage(const sensr_proto::OutputMessage &message) {
        //INFO_LOG("Deprecated Message");
    }

    void MessageListener::OnGetOutputMessage(const sensr_proto::OutputMessage &message) {
        INFO_LOG("New OutputMessage");
    }

    void MessageListener::OnGetPointResult(const sensr_proto::PointResult &message) {
        INFO_LOG("New PointResult");
    }

    bool MessageListener::IsOutputMessageListening() const {
        return static_cast<uint32_t>(listening_type_) & static_cast<uint32_t>(ListeningType::kOutputMessage);
    }

    bool MessageListener::IsPointResultListening() const {
        return static_cast<uint32_t>(listening_type_) & static_cast<uint32_t>(ListeningType::kPointResult);
    }
}