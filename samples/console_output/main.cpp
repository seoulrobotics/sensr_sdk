#include "sensr.h"
#include <iostream>
#include <google/protobuf/util/time_util.h>



class SampleListener : public sensr::MessageListener {
public:
  SampleListener(sensr::Client* client) : MessageListener(ListeningType::kBoth), client_(client) {

  }
  void OnError(Error error, const std::string& reason) {
    if (error == sensr::MessageListener::Error::kOutputMessageConnection || 
      error == sensr::MessageListener::Error::kPointResultConnection ) {
      client_->Reconnect();
    }
  }

  void OnGetOutpuMessage(const sensr_proto::OutputMessage &message) {
    // if (message.has_stream()) {
    //   for(const auto& object : message.stream().objects()) {
    //     int object_points_size = (object.points().length() / (sizeof(float) * 3));
    //     std::cout << "Obj(" << object.id() << ") : point no. " << object_points_size << std::endl;
    //   }
    // }

    if (message.has_event()) {
      for(const auto& zone_event : message.event().zone()) {
        if (zone_event.type() == sensr_proto::ZoneEvent_Type_ENTRY) {
          std::cout << "Entering Zone(" << zone_event.id() << ") : obj( " << zone_event.object().id() << ")" << std::endl;
        } else if (zone_event.type() == sensr_proto::ZoneEvent_Type_EXIT) {
          std::cout << "Exiting Zone(" << zone_event.id() << ") : obj( " << zone_event.object().id() << ")" << std::endl;
        }
      }
    }
    
    //std::cout << "Message received from SENSR!" << std::endl
    //          << "Timestamp: " << google::protobuf::util::TimeUtil::ToString(message.time_stamp()) << std::endl;
              /*<< "Ground Points: " << message.ground_points().length() / (sizeof(float) * 3) << std::endl
              << "Object Points: " << object_points_size << std::endl
              << "Tracked Objects: " << tracked_objects_size << std::endl
              << "Non Tracked Objects: " << non_tracked_objects_size << std::endl
              << std::endl;*/
  }

  void OnGetPointResult(const sensr_proto::PointResult &message) {
    for(const auto& point_cloud : message.points()) {
        if (point_cloud.type() == sensr_proto::PointResult_PointCloud_Type_RAW) {
          std::cout << "Topic(" << point_cloud.id() << ") no. of points - " << point_cloud.points().length() / (sizeof(float) * 3) << std::endl;
        } else if (point_cloud.type() == sensr_proto::PointResult_PointCloud_Type_GROUND) {
          std::cout << "Gound points no. of points - " << point_cloud.points().length() / (sizeof(float) * 3) << std::endl;
        } else if (point_cloud.type() == sensr_proto::PointResult_PointCloud_Type_ENVIRONMENT) {
          std::cout << "Environment points no. of points - " << point_cloud.points().length() / (sizeof(float) * 3) << std::endl;
        }
      }
  }
private:
  sensr::Client* client_;
};

int main(int argc, char *argv[])
{
  const char *client_address = "localhost";
  if (argc > 1)
  {
    client_address = argv[1];
  }
  std::string address = std::string(client_address);
  sensr::Client client(address);
  std::shared_ptr<sensr::MessageListener> listener = std::make_shared<SampleListener>(&client);
  client.SubscribeMessageListener(listener);
  std::string s;
  std::getline(std::cin, s);
  while(s != "") { // if the person hits enter, s == "" and leave the loop
      std::cout << s << std::endl;
      std::getline(std::cin, s);
  }
  client.UnsubscribeMessageListener(listener);
  return 0;
}