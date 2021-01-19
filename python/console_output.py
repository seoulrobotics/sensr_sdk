from sensr_message_listener import MessageListener, ListenerType

# TODO remove this comment
# class ZoneEventListener : public sensr::MessageListener {
# public:
#   ZoneEventListener(sensr::Client* client) : MessageListener(ListeningType::kOutputMessage), client_(client) {}
#   void OnError(Error error, const std::string& reason) {
#     if (error == sensr::MessageListener::Error::kOutputMessageConnection || 
#       error == sensr::MessageListener::Error::kPointResultConnection ) {
#       client_->Reconnect();
#     }
#   }
#   void OnGetOutpuMessage(const sensr_proto::OutputMessage &message) {
#     if (message.has_event()) {
#       for(const auto& zone_event : message.event().zone()) {
#         if (zone_event.type() == sensr_proto::ZoneEvent_Type_ENTRY) {
#           std::cout << "Entering Zone(" << zone_event.id() << ") : obj( " << zone_event.object().id() << ")" << std::endl;
#         } else if (zone_event.type() == sensr_proto::ZoneEvent_Type_EXIT) {
#           std::cout << "Exiting Zone(" << zone_event.id() << ") : obj( " << zone_event.object().id() << ")" << std::endl;
#         }
#       }
#     }
#   }
# private:
#   sensr::Client* client_;
# };

class ZoneEvenListener(MessageListener):

    def __init__(self, address):
        super().__init__(address=address, 
                         listener_type=ListenerType.OUTPUT_MESSAGE)

    def _on_get_output_message(self, message):
        print("Got output")

    def _on_get_point_result(self, message):
        print("Got points")






if __name__ == "__main__":
    
    address = "localhost"
    zone_listener = ZoneEvenListener(address)
    zone_listener.connect()