#include "sensr.h"
#include <iostream>


void ReceiveOutputMessage(const sensr_proto::OutputMessage& message) {
   /*for(const auto& object : message.objects()) {
    object_points_size += (object.points().length() / (sizeof(float) * 3));
    if (object.has_track() && object.track().tracking_reliable()) {
      tracked_objects_size++;
    } else {
      non_tracked_objects_size++;
    }
  }*/
  std::cout << "Message received from SENSR!" << std::endl
            << "Timestamp: " << message.time_stamp().seconds() << " (s) "
                              << message.time_stamp().nanos() << " (ns)" << std::endl;
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
  sensr::Client client;
  //auto func = std::bind(&ReceiveOutputMessage, std::placeholders::_1);
  client.SubscribeMessageListener(address, &ReceiveOutputMessage);
  std::string s;
  std::getline(std::cin, s);
  while(s != "") { // if the person hits enter, s == "" and leave the loop
      std::cout << s << std::endl;
      std::getline(std::cin, s);
  }
  client.UnsubscribeMessageListener(address, &ReceiveOutputMessage);
  return 0;
}