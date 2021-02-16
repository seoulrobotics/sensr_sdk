#include "sensr_message_listener.h"
#include <iostream>

namespace sensr {
    MessageListener::MessageListener(ListeningType listening_type) 
    : listening_type_(listening_type) {

    }

    void MessageListener::OnError(MessageListener::Error error, const std::string& reason) {
        switch (error) {
            case Error::kOutputMessageConnection: 
            case Error::kPointResultConnection: {
                std::cerr << "Lost SENSR Connection fail(Reason: " << reason << "). Please reconnect." << std::endl; 
                break;
            }
            default:
                break;
        }
    }

    void MessageListener::OnGetOutpuMessage(const sensr_proto::OutputMessage &message) {
        std::cout << "Deprecated Message" << std::endl;
    }

    void MessageListener::OnGetOutputMessage(const sensr_proto::OutputMessage &message) {
        std::cout << "New OutputMessage" << std::endl;
    }

    void MessageListener::OnGetPointResult(const sensr_proto::PointResult &message) {
        std::cout << "New PointResult" << std::endl;
    }

    bool MessageListener::IsOutputMessageListening() const {
        return static_cast<uint32_t>(listening_type_) & static_cast<uint32_t>(ListeningType::kOutputMessage);
    }

    bool MessageListener::IsPointResultListening() const {
        return static_cast<uint32_t>(listening_type_) & static_cast<uint32_t>(ListeningType::kPointResult);
    }
}