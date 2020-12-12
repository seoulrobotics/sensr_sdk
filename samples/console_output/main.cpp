#include "sensr.h"
#include <iostream>
#include <google/protobuf/util/time_util.h>

void ReceiveOutputMessage(const sensr_proto::OutputMessage& message) {
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

int main(int argc, char *argv[])
{
  const char *client_address = "localhost";
  if (argc > 1)
  {
    client_address = argv[1];
  }
  std::string address = std::string(client_address);
  sensr::Client client(address);
  client.SubscribeMessageListener(&ReceiveOutputMessage);
  std::string s;
  std::getline(std::cin, s);
  while(s != "") { // if the person hits enter, s == "" and leave the loop
      std::cout << s << std::endl;
      std::getline(std::cin, s);
  }
  client.UnsubscribeMessageListener();
  return 0;
}