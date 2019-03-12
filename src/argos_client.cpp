#include "argos_client.h"

const int MAX_ADDRESS_LEN = 512;

namespace argos
{
Client::Client(const char *address)
    : context_(), subscriber_(context_, ZMQ_SUB)
{
  char buffer[MAX_ADDRESS_LEN];
  int result = snprintf(&buffer[0], MAX_ADDRESS_LEN, "tcp://%s:5050", address);
  if( result > 0 )
  { 
    subscriber_.connect(buffer);
    subscriber_.setsockopt(ZMQ_SUBSCRIBE, "", 0);
  }
}

Client::~Client()
{
}

bool Client::ReceiveMessageAsync(output_message &message)
{
  zmq::message_t network_message;
  if (subscriber_.recv(&network_message, ZMQ_DONTWAIT))
  {
    std::string msg_string(network_message.data<char>(), network_message.size());
    message.ParseFromString(msg_string);
    return true;
  }
  return false;
}

bool Client::ReceiveMessage(output_message &message)
{
  zmq::message_t network_message;
  if (subscriber_.recv(&network_message, 0))
  {
    std::string msg_string(network_message.data<char>(), network_message.size());
    message.ParseFromString(msg_string);
    return true;
  }
  return false;
}

} // namespace argos
