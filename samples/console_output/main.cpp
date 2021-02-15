#include "sensr.h"
#include <iostream>
#include <cstring>
#if defined(__linux__)
#include <sys/time.h>
#endif
#include <google/protobuf/util/time_util.h>

class ZoneEventListener : public sensr::MessageListener {
public:
  ZoneEventListener(sensr::Client* client) : MessageListener(ListeningType::kOutputMessage), client_(client) {}
  void OnError(Error error, const std::string& reason) {
    if (error == sensr::MessageListener::Error::kOutputMessageConnection || 
      error == sensr::MessageListener::Error::kPointResultConnection ) {
      client_->Reconnect();
    }
  }
  void OnGetOutpuMessage(const sensr_proto::OutputMessage &message) {
    if (message.has_event()) {
      for(const auto& zone_event : message.event().zone()) {
        if (zone_event.type() == sensr_proto::ZoneEvent_Type_ENTRY) {
          std::cout << "Entering Zone(" << zone_event.id() << ") : obj( " << zone_event.object().id() << ")" << std::endl;
        } else if (zone_event.type() == sensr_proto::ZoneEvent_Type_EXIT) {
          std::cout << "Exiting Zone(" << zone_event.id() << ") : obj( " << zone_event.object().id() << ")" << std::endl;
        }
      }
    }
  }
private:
  sensr::Client* client_;
};

class PointResultListener : public sensr::MessageListener {
public:
  PointResultListener(sensr::Client* client) : MessageListener(ListeningType::kPointResult), client_(client) {}
  void OnError(Error error, const std::string& reason) {
    if (error == sensr::MessageListener::Error::kOutputMessageConnection || 
      error == sensr::MessageListener::Error::kPointResultConnection ) {
      client_->Reconnect();
    }
  }
  void OnGetPointResult(const sensr_proto::PointResult &message) {
    for(const auto& point_cloud : message.points()) {
      if (point_cloud.type() == sensr_proto::PointResult_PointCloud_Type_RAW) {
        std::cout << "Topic(" << point_cloud.id() << ") no. of points - " << point_cloud.points().length() / (sizeof(float) * 3) << std::endl;
      } else if (point_cloud.type() == sensr_proto::PointResult_PointCloud_Type_GROUND) {
        std::cout << "Gound points no. of points - " << point_cloud.points().length() / (sizeof(float) * 3) << std::endl;
      } else if (point_cloud.type() == sensr_proto::PointResult_PointCloud_Type_BACKGROUND) {
        std::cout << "Background points no. of points - " << point_cloud.points().length() / (sizeof(float) * 3) << std::endl;
      }
    }
  }
private:
  sensr::Client* client_;
};

class ObjectListener : public sensr::MessageListener {
public:
  ObjectListener(sensr::Client* client) : MessageListener(ListeningType::kOutputMessage), client_(client) {}
  void OnError(Error error, const std::string& reason) {
    if (error == sensr::MessageListener::Error::kOutputMessageConnection || 
      error == sensr::MessageListener::Error::kPointResultConnection ) {
      client_->Reconnect();
    }
  }
  void OnGetOutpuMessage(const sensr_proto::OutputMessage &message) {
    if (message.has_stream()) {
      for(const auto& object : message.stream().objects()) {
        int object_points_size = (object.points().length() / (sizeof(float) * 3));
        std::cout << "Obj(" << object.id() << ") : point no. " << object_points_size << std::endl;
      }
    }
  }
private:
  sensr::Client* client_;
};

class HealthListener : public sensr::MessageListener {
public:
  HealthListener(sensr::Client* client) : MessageListener(ListeningType::kOutputMessage), client_(client) {}
  void OnError(Error error, const std::string& reason) {
    if (error == sensr::MessageListener::Error::kOutputMessageConnection || 
      error == sensr::MessageListener::Error::kPointResultConnection ) {
      client_->Reconnect();
    }
  }
  void OnGetOutpuMessage(const sensr_proto::OutputMessage &message) {
    if (message.has_stream() && message.stream().has_health()) {
      auto system_health = message.stream().health();
      std::cout << "System health: " << system_health.master() << std::endl;
      for(const auto& node : system_health.nodes()) {
        auto node_health = node.second;
        std::cout << "Node("<< node.first <<") health: " << node_health.status() << std::endl;
        for (const auto& sensor : node_health.sensors()) {
          std::cout << "Sensor("<< sensor.first <<") health: " << sensor.second << std::endl;
        }
      }
    }
  }
private:
  sensr::Client* client_;
};

class TimeChecker : public sensr::MessageListener {
public:
  TimeChecker(sensr::Client* client) : MessageListener(ListeningType::kOutputMessage), client_(client) {}
  void OnError(Error error, const std::string& reason) {
    if (error == sensr::MessageListener::Error::kOutputMessageConnection || 
      error == sensr::MessageListener::Error::kPointResultConnection ) {
      client_->Reconnect();
    }
  }
  void OnGetOutpuMessage(const sensr_proto::OutputMessage &message) {
  #if defined(__linux__)
    timeval msg_tv = google::protobuf::util::TimeUtil::TimestampToTimeval(message.timestamp());
    timeval tv;
    gettimeofday(&tv, nullptr);
    timeval diff;
    timersub(&tv, &msg_tv, &diff);
    std::cout << "Diff:" << diff.tv_usec / (float)1000 << "ms" << std::endl;
  #endif
              
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
  // Add sample listeners
  if (argc > 2) {
    for (int i = 2; i < argc; ++i) {
      std::shared_ptr<sensr::MessageListener> listener;
      if(strcmp(argv[i], "zone") == 0) {
        listener = std::make_shared<ZoneEventListener>(&client);
      } else if (strcmp(argv[i], "point") == 0) {
        listener = std::make_shared<PointResultListener>(&client);
      } else if (strcmp(argv[i], "object") == 0) {
        listener = std::make_shared<ObjectListener>(&client);
      } else if (strcmp(argv[i], "time") == 0) {
        listener = std::make_shared<TimeChecker>(&client);
      } else if (strcmp(argv[i], "health") == 0) {
        listener = std::make_shared<HealthListener>(&client);
      }
      if (listener) {
        client.SubscribeMessageListener(listener);
      }
    }
  } else {
    // Default sample listeners
    std::shared_ptr<sensr::MessageListener> listener = std::make_shared<ZoneEventListener>(&client);
    client.SubscribeMessageListener(listener);
  }
  std::string s;
  std::getline(std::cin, s);
  while(s != "") { // if the person hits enter, s == "" and leave the loop
      std::cout << s << std::endl;
      std::getline(std::cin, s);
  }
  return 0;
}