#include "argos.h"

int main(int argc, char *argv[])
{
  const char *client_address = "localhost";
  if (argc > 1)
  {
    client_address = argv[1];
  }
  argos::Client client(client_address);

  while (true)
  {
    output_message message;
    if (client.ReceiveMessage(message))
    {
      std::cout << "Message received from argos!" << std::endl;
    }
  }
}