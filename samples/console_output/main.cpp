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
      std::cout << "Message received from SENSR!" << std::endl;
    }
  }
}