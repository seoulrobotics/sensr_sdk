#include "sensr.h"
#include <iostream>

int main(int argc, char *argv[])
{
  const char *client_address = "192.168.77.3";
  if (argc > 1)
  {
    client_address = argv[1];
  }
  sensr::Client client(client_address);

  while (true)
  {
    OutputMessage message;
    if (client.ReceiveMessage(message))
    {
      size_t object_points_size = 0;
      int tracked_objects_size = 0, non_tracked_objects_size = 0;
      for(const auto& object : message.objects()) {
        object_points_size += (object.points().length() / (sizeof(float) * 3));
        if (object.has_track() && object.track().tracking_reliable()) {
          tracked_objects_size++;
        } else {
          non_tracked_objects_size++;
        }
      }
      std::cout << "Message received from SENSR!" << std::endl
                << "Timestamp: " << message.time_stamp().seconds() << " (s) "
                                 << message.time_stamp().nanos() << " (ns)" << std::endl
                << "Ground Points: " << message.ground_points().length() / (sizeof(float) * 3) << std::endl
                << "Object Points: " << object_points_size << std::endl
                << "Tracked Objects: " << tracked_objects_size << std::endl
                << "Non Tracked Objects: " << non_tracked_objects_size << std::endl
                << std::endl;
    }
  }
}