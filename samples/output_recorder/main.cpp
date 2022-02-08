#include "sensr.h"
#include "math.h"
#include <iostream>
#include <fstream>
#include <cstring>
#if defined(__linux__)
#include <sys/time.h>
#include <sys/stat.h>
#endif
#include <google/protobuf/util/time_util.h>
#include "replay.pb.h"

std::array<std::string, 7> labels = {"NONE", "CAR", "PED", "CYC", "MISC", "GROUND", "MAX"};
class MessageRecorder : public sensr::MessageListener {
  
public:
  MessageRecorder(sensr::Client* client, const std::string& result_file_name, const std::string& folder_name)
  : MessageListener(ListeningType::kOutputMessage)
  , file_name_(result_file_name)
  , client_(client)
  , folder_name_(folder_name) { }

  const size_t GetFrameCount() const { return static_cast<size_t>(output_messages_.output_messages_size()); }

  void SaveRecordedMessage() {
    size_t frame_idx = 0u;
    auto output_folder = "/home/seoulrobotics/data/Data_Label/" + folder_name_ + "/output";
    mkdir(output_folder.c_str(), 0777);
    for (int i=0; i<output_messages_.output_messages_size(); ++i) {
      auto frame_str = std::to_string(frame_idx);
      std::ofstream record_file(output_folder + "/" + file_name_ + frame_str.insert(0u, 3u - frame_str.size(), '0') + ".txt", std::ios::out | std::ios::app);
      auto& output_message = output_messages_.output_messages(i);
      auto& stream = output_message.stream();
      auto& event = output_message.event();
      auto& custom = output_message.custom();

      for (auto& object : stream.objects()) {
        auto& bbox = object.bbox();
        auto yaw = bbox.yaw();
        if (yaw > M_PI) {
          yaw -= 2 * M_PI;
        }
        auto& position = bbox.position();
        auto& size = bbox.size();
        record_file<<object.id()<<" "<<position.x()<<" "<<position.y()<<" "<<0.00f<<" "<<
        size.x()<<" "<<size.y()<<" "<<size.z()<<" "<<yaw<<" "<<labels[object.label()]<<std::endl;
      }
      record_file.close();
      ++frame_idx;
    }
    
    // output_messages_.SerializeToOstream(&record_file);
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
      if (output_messages_.output_messages().empty()) {
        output_messages_.mutable_timestamp()->CopyFrom(message.timestamp());
      }
      output_messages_.add_output_messages()->CopyFrom(message);
      std::cout << "Message received from SENSR!" << std::endl
                << "Timestamp: " << message.timestamp().seconds() << " (s) "
                                << message.timestamp().nanos() << " (ns)" << std::endl
                << "Object Points: " << message.stream().objects_size() << std::endl
                << "frame count: " << output_messages_.output_messages_size() << std::endl;
      
    }
  }

private:
  std::string file_name_;
  std::string folder_name_;
  sensr::Client* client_;
  ReplayMessage output_messages_;
};

int main(int argc, char *argv[])
{
  if(argc < 5) {
    std::cerr << "Usage: " << argv[0] << " <NUMBER_OF_FRAMES>" << " <CLIENT_ADDRESS>" << " <OUTPUT_FILE_NAME>" << " <OUTPUT_FOLDER_NAME>" << std::endl;
    return 1;
  } else {
    std::cout << " <NUMBER_OF_FRAMES>: " << argv[1] << std::endl;
    std::cout << " <CLIENT_ADDRESS>: " << argv[2] << std::endl;
    std::cout << " <OUTPUT_FILE_NAME>: " << argv[3] << std::endl;
    std::cout << " <OUTPUT_FOLDER_NAME>: " << argv[4] << std::endl;
  }
  const char* client_address = argv[2];
  std::string address = std::string(client_address);
  sensr::Client client(address);

  std::shared_ptr<sensr::MessageListener> listener = std::make_shared<MessageRecorder>(&client, std::string(argv[3]), std::string(argv[4]));
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
