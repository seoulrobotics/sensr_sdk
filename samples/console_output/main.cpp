#include "sensr.h"
#include <iostream>
#include <cstring>
#include <algorithm>
#if defined(__linux__)
#include <sys/time.h>
#endif
#include <google/protobuf/util/time_util.h>

std::vector<float> BytesToFloatVector(const std::string& bytes) {
  std::vector<float> output;
  output.resize(bytes.length() / sizeof(float));
  memcpy(output.data(), bytes.data(), bytes.length());
  return output;
}

class ZoneEventListener : public sensr::MessageListener {
public:
  ZoneEventListener(sensr::Client* client) : MessageListener(ListeningType::kOutputMessage), client_(client) {}
  void OnError(Error error, const std::string& reason) override {
    std::cerr << reason << std::endl;
    if (error == sensr::MessageListener::Error::kOutputMessageConnection || 
      error == sensr::MessageListener::Error::kPointResultConnection ||
      error == sensr::MessageListener::Error::kOutputBufferOverflow ) {
      client_->Reconnect();
    }
  }
  void OnGetOutputMessage(const sensr_proto::OutputMessage &message) override {
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
  void OnError(Error error, const std::string& reason) override {
    std::cerr << reason << std::endl;
    if (error == sensr::MessageListener::Error::kOutputMessageConnection || 
      error == sensr::MessageListener::Error::kPointResultConnection ||
      error == sensr::MessageListener::Error::kOutputBufferOverflow ) {
      client_->Reconnect();
    }
  }
  void OnGetPointResult(const sensr_proto::PointResult &message) override {
    for(const auto& point_cloud : message.points()) {      

      if (point_cloud.type() == sensr_proto::PointResult_PointCloud_Type_RAW) {
        std::cout << "Topic(" << point_cloud.id() << ") no. of points - " << point_cloud.points().length() / (sizeof(float) * 3) << std::endl;
      } else if (point_cloud.type() == sensr_proto::PointResult_PointCloud_Type_GROUND) {
        std::cout << "Gound points no. of points - " << point_cloud.points().length() / (sizeof(float) * 3) << std::endl;
      } else if (point_cloud.type() == sensr_proto::PointResult_PointCloud_Type_BACKGROUND) {
        std::cout << "Background points no. of points - " << point_cloud.points().length() / (sizeof(float) * 3) << std::endl;
      }

      std::vector<float> intensities = BytesToFloatVector(point_cloud.intensities());
      std::cout << "Intensity [min, median, max] is [";
      std::cout << *std::min_element(std::begin(intensities), std::end(intensities)) << ", ";
      const int mid = intensities.size()/2;
      std::nth_element(std::begin(intensities), std::begin(intensities)+mid, std::end(intensities));
      std::cout << intensities[mid] << ", ";
      std::cout << *std::max_element(std::begin(intensities), std::end(intensities)) << "].\n";
    }
  }
private:
  sensr::Client* client_;
};

class ObjectListener : public sensr::MessageListener {
public:
  ObjectListener(sensr::Client* client) : MessageListener(ListeningType::kOutputMessage), client_(client) {}
  void OnError(Error error, const std::string& reason) override {
    std::cerr << reason << std::endl;
    if (error == sensr::MessageListener::Error::kOutputMessageConnection || 
      error == sensr::MessageListener::Error::kPointResultConnection ||
      error == sensr::MessageListener::Error::kOutputBufferOverflow) {
      client_->Reconnect();
    }
  }
  void OnGetOutputMessage(const sensr_proto::OutputMessage &message) override {
    if (message.has_stream()) {
      for(const auto& object : message.stream().objects()) {
        if (object.has_history()) {
          std::cout << "Obj(" << object.id() << "):";
          for (auto state : object.history().states()) {
            std::cout << "(" << state.position().x() << ", " << state.position().y() << ", " << state.position().z() << ") :  "
             << google::protobuf::util::TimeUtil::ToString(state.timestamp()) << std::endl;
          }
          std::cout << std::endl;
        }	

        //box size
        if(object.has_bbox())
        {
          std::cout << "Obj(" << object.id() << "): ";
          std::cout << "bbox Position: ["<< object.bbox().position().x() << "," << object.bbox().position().y() << "," 
            << object.bbox().position().z() << "], ";

          //box yaw
          std::cout << "bbox yaw: " << object.bbox().yaw() << std::endl;
          
        }

        //velocity
        if(object.has_velocity()) 
        {
          std::cout << "Obj(" << object.id() << "): ";
          std::cout << "velocity: [" << object.velocity().x() << "," << object.velocity().y() << "," 
            << object.velocity().z() << "]" << std::endl;
        }
        
        //tracking status
        std::cout << "Obj(" << object.id() << "): ";
        std::cout << "tracking status: " << TrackingStatus_Name(object.tracking_status()) << std::endl;;

        //classification result
        std::cout << "Obj(" << object.id() << "): ";
        std::cout << "classification result: " << LabelType_Name(object.label()) << std::endl;
        
        //probability
        std::cout << "Obj(" << object.id() << "): ";
        for(auto pred : object.prediction().positions())
        {
          std::cout << "prediction: [" << pred.x() << "," << pred.y() << "," << pred.z() << "]";
        }
        std::cout << std::endl;
     }
    }
  }
private:
  sensr::Client* client_;
};

class HealthListener : public sensr::MessageListener {
public:
  HealthListener(sensr::Client* client) : MessageListener(ListeningType::kOutputMessage), client_(client) {}
  void OnError(Error error, const std::string& reason) override {
    std::cerr << reason << std::endl;
    if (error == sensr::MessageListener::Error::kOutputMessageConnection || 
      error == sensr::MessageListener::Error::kPointResultConnection ||
      error == sensr::MessageListener::Error::kOutputBufferOverflow) {
      client_->Reconnect();
    }
  }
  void OnGetOutputMessage(const sensr_proto::OutputMessage &message) override {
    if (message.has_stream() && message.stream().has_health()) {
      auto system_health = message.stream().health();
      std::cout << "System health: " << system_health.master() << std::endl;
      for(const auto& node : system_health.nodes()) {
        auto node_health = node.second;
        std::cout << "Node("<< node.first <<") health: " << node_health.status() << std::endl;
        for (const auto& sensor : node_health.sensors()) {
          std::cout << "Sensor("<< sensor.first <<") health: " << sensor.second.DebugString() << std::endl;
        }
      }
    } else if (message.has_custom()) {
      std::cout << "BG Learn : " << message.custom().bg_learning_progress() << std::endl;
    }
  }
private:
  sensr::Client* client_;
};
class TimeChecker : public sensr::MessageListener {
public:
  TimeChecker(sensr::Client* client) : MessageListener(ListeningType::kOutputMessage), client_(client) {}
  void OnError(Error error, const std::string& reason) override {
    std::cerr << reason << std::endl;
    if (error == sensr::MessageListener::Error::kOutputMessageConnection || 
      error == sensr::MessageListener::Error::kPointResultConnection ||
      error == sensr::MessageListener::Error::kOutputBufferOverflow) {
      client_->Reconnect();
    }
  }
  void OnGetOutputMessage(const sensr_proto::OutputMessage &message) override {
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

class ProfilingChecker : public sensr::MessageListener {
public:
  ProfilingChecker(sensr::Client* client) : MessageListener(ListeningType::kOutputMessage), client_(client) {}
  void OnError(Error error, const std::string& reason) override {
    std::cerr << reason << std::endl;
    if (error == sensr::MessageListener::Error::kOutputMessageConnection || 
      error == sensr::MessageListener::Error::kPointResultConnection ||
      error == sensr::MessageListener::Error::kOutputBufferOverflow) {
      client_->Reconnect();
    }
  }
  void OnGetOutputMessage(const sensr_proto::OutputMessage &message) override {
    if (message.has_custom() && message.custom().has_profiling()) {
      auto& master_proc_infos = message.custom().profiling().master_node().processing_times();
      std::cout << "Master node overall : " << master_proc_infos.at("UIRuntimeState-OnUpdate") << std::endl;
      for (auto& node : message.custom().profiling().algo_nodes()) {
        auto uid = node.first;
        auto node_proc_infos = node.second.processing_times();
        std::cout << uid << " node overall : " << node_proc_infos.at("FrameOverall") << std::endl;      
      }
    }              
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
  // If you want to use Secured output, please pass correct certificate path.
  // e.g.)  sensr::Client client(address, "$HOME/seoulrobotics/keys/sensr-ca.crt");
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
      } else if (strcmp(argv[i], "profile") == 0) {
        listener = std::make_shared<ProfilingChecker>(&client);
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