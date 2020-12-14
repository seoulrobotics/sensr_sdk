#pragma once

#include "sensr_proto/output.pb.h"
#include "sensr_proto/point_cloud.pb.h"
#include <string>

namespace sensr {
  class MessageListener {
  public:
    enum struct ListeningType : uint32_t {
        kNone = 0x00000000,
        kOutputMessage = 0x00000001,
        kPointResult = 0x00000002,
        kBoth = (kOutputMessage | kPointResult)
    };
    enum struct Error {
        kNone,
        kOutputMessageConnection,
        kPointResultConnection,
        kMax
    };

    MessageListener(ListeningType listening_type = ListeningType::kOutputMessage);
    virtual ~MessageListener() = default;
    virtual void OnError(Error error, const std::string& reason);
    virtual void OnGetOutpuMessage(const sensr_proto::OutputMessage &message) = 0;
    virtual void OnGetPointResult(const sensr_proto::PointResult &message) = 0;
    bool IsOutputMessageListening() const;
    bool IsPointResultListening() const;
  private:
    ListeningType listening_type_;
  };
} // namespace sensr
