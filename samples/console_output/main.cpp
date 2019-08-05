#include "sensr.h"
#include <iostream>

int main(int argc, char *argv[])
{
  const char *client_address = "localhost";
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
                << "Timestamp: " << latest_message.time_stamp().seconds() << " (s) "
                                 << latest_message.time_stamp().nano_seconds() " (ns)" << std::endl
                << "Ground Points: " << latest_message.point_cloud().ground_points().size() << std::endl
                << "Object Points: " << latest_message.point_cloud().object_points().size() << std::endl
                << "Invalid Points: " << latest_message.point_cloud().invalid_points().size() << std::endl
                << "Tracked Objects: " << latest_message.tracked_objects_size() << std::endl
                << "Non Tracked Objects: " << latest_message.non_tracked_objects_size() << std::endl
                << std::endl;
    }
  }
}