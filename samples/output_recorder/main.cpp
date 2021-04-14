#include "sensr.h"
#include <iostream>
#include <fstream>
#include <cstring>
#if defined(__linux__)
#include <sys/time.h>
#endif
#include <google/protobuf/util/time_util.h>

class MessageRecorder : public sensr::MessageListener {
public:
  MessageRecorder(sensr::Client* client, const std::string& result_file_name)
  : MessageListener(ListeningType::kOutputMessage)
  , file_name_(result_file_name)
  , client_(client) { }

  const size_t GetFrameCount() const { return static_cast<size_t>(output_messages_.output_messages_size()); }

  void SaveRecordedMessage() {
    std::ofstream record_file(file_name_ + ".re");
    output_messages_.SerializeToOstream(&record_file);
    std::cout << "Output is saved successfully...." << std::endl;
  }

  void OnError(Error error, const std::string& reason) {
    if (error == sensr::MessageListener::Error::kOutputMessageConnection || 
      error == sensr::MessageListener::Error::kPointResultConnection ) {
      client_->Reconnect();
    }
  }

  void OnGetOutpuMessage(const sensr_proto::OutputMessage &message) {
    if (message.has_stream()) {
      output_messages_.add_output_messages()->CopyFrom(message);
      std::cout << "Message received from SENSR!" << std::endl
                << "Timestamp: " << message.time_stamp().seconds() << " (s) "
                                << message.time_stamp().nanos() << " (ns)" << std::endl
                << "Object Points: " << message.stream().objects_size() << std::endl
                << "frame count: " << output_messages_.output_messages_size() << std::endl;
      
    }
  }

private:
  std::string file_name_;
  sensr::Client* client_;
  sensr_proto::ReplayMessage output_messages_;
};

int main(int argc, char *argv[])
{
  if(argc < 4) {
    std::cerr << "Usage: " << argv[0] << " <NUMBER_OF_FRAMES>" << " <CLIENT_ADDRESS>" << " <OUTPUT_FILE_NAME>" << std::endl;
    return 1;
  } else {
    std::cout << " <NUMBER_OF_FRAMES>: " << argv[1] << std::endl;
    std::cout << " <CLIENT_ADDRESS>: " << argv[2] << std::endl;
    std::cout << " <OUTPUT_FILE_NAME>: " << argv[3] << std::endl;
  }
  const char* client_address = argv[2];
  std::string address = std::string(client_address);
  sensr::Client client(address, "keys/sensr-sdk-ca.crt");

  std::shared_ptr<sensr::MessageListener> listener = std::make_shared<MessageRecorder>(&client, std::string(argv[3]));
  client.SubscribeMessageListener(listener);
   while(true) {
    if(std::dynamic_pointer_cast<MessageRecorder>(listener)->GetFrameCount() >= std::stoul(argv[1])) {
      client.UnsubscribeMessageListener(listener);
      std::dynamic_pointer_cast<MessageRecorder>(listener)->SaveRecordedMessage();
      break;
    }
  }
  return 0;
}