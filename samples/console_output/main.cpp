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
    output_message message;
    if (client.ReceiveMessage(message))
    {
      std::cout << "Message received from SENSR!" << std::endl
                << "Timestamp: " << message.time_stamp().seconds() << " (s) "
                                 << message.time_stamp().nano_seconds() << " (ns)" << std::endl
                << "Ground Points: " << message.point_cloud().ground_points().size() << std::endl
                << "Object Points: " << message.point_cloud().object_points().size() << std::endl
                << "Tracked Objects: " << message.tracked_objects_size() << std::endl
                << "Non Tracked Objects: " << message.non_tracked_objects_size() << std::endl
                << std::endl;
    }
  }
}