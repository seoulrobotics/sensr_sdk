#include "sensr.h"
#include <iostream>
#include <fstream>
#include <cstring>
#if defined(__linux__)
#include <sys/time.h>
#include <sys/stat.h>
#include <chrono>
#endif
#include <google/protobuf/util/time_util.h>
#include <queue>

class ZoneEventListener : public sensr::MessageListener {
  using sys_clock = std::chrono::system_clock;
public:
  ZoneEventListener(sensr::Client* client) : MessageListener(ListeningType::kOutputMessage), client_(client), failure_cnt_(0u) {
    save_log_.open("fifo_failure_log.txt");
    prev_time_ = sys_clock::now();
  }
  ~ZoneEventListener() {
    PrintElements();
    save_log_.close();
  }
  void SetZone(int entrance, int exit) {
    zone_entrance_ = entrance;
    zone_exit_ = exit;
  }
  void PrintElements() {
    std::cout<<"Print cars on the highway."<<std::endl;
    for (const auto& id : cars_on_highway_) {
      std::cout<<"id: "<<id<<" "<<std::endl;
    }
  }
  void OnError(Error error, const std::string& reason) {
    (void)reason;
    if (error == sensr::MessageListener::Error::kOutputMessageConnection || 
      error == sensr::MessageListener::Error::kPointResultConnection ) {
      client_->Reconnect();
    }
  }
  void SaveCurrentCarsOnHighway() {
    auto now = std::chrono::system_clock::now();
    std::time_t now_t = std::chrono::system_clock::to_time_t(now);
    std::string now_str = std::ctime(&now_t);
    if (car_logs_.size() > 5u) {
      car_logs_.pop_front();
    }
    car_logs_.emplace_back(now_str, cars_on_highway_);
  }
  void SaveLog(int zone_obj_id) {
    auto now = std::chrono::system_clock::now();
    std::time_t now_t = std::chrono::system_clock::to_time_t(now);
    save_log_ << "[" + std::to_string(failure_cnt_) + "]" <<"FIFO failed. logs are saved."<<std::endl;
    save_log_ << "System_time : " << std::ctime(&now_t);
    save_log_ << "Zone entered id : " << zone_obj_id << std::endl;
    save_log_ << std::endl;
    save_log_ << "Car logs" << std::endl;
    for (const auto& car_log : car_logs_) {
      save_log_ << "Logged time: " << car_log.first;
      for (const auto& car : car_log.second) {
        save_log_<<"id : "<< car << std::endl;
      }
      save_log_ << std::endl;
    }
    save_log_ << "Current Cars On the Highway" << std::endl;
    for (const auto& car : cars_on_highway_) {
      save_log_ << "id : " << car << std::endl;
    }
    save_log_ << std::endl;
  }
  void OnGetOutpuMessage(const sensr_proto::OutputMessage &message) {
    auto now = sys_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::seconds>(now - prev_time_);
    if (duration.count() >= 20) {
      SaveCurrentCarsOnHighway();
      prev_time_ = now;
    }

    if (message.has_event()) {
      auto objects = message.stream().objects();
      for(const auto& zone_event : message.event().zone()) {
        int zone_id = zone_event.id();
        if (zone_id != zone_entrance_ && zone_id != zone_exit_) {
          continue;
        }
        int zone_obj_id = zone_event.object().id();
        bool is_tracked_car = false;
        for (const auto& object : objects) {
          int id = object.id();
          if (zone_obj_id == id && object.label() == sensr_proto::LabelType::LABEL_CAR 
            && object.tracking_status() == sensr_proto::TrackingStatus::TRACKING) {
            is_tracked_car = true;
            break;
          } 
        }
        if (is_tracked_car && zone_event.type() == sensr_proto::ZoneEvent_Type_ENTRY) {
          std::cout << "Entering Zone(" << zone_event.id() << ") : obj( " << zone_event.object().id() << ")" << std::endl;
          auto iter = std::find(cars_on_highway_.begin(), cars_on_highway_.end(), zone_obj_id); 
          if (iter == cars_on_highway_.end()) {
            cars_on_highway_.emplace_back(zone_obj_id);
          } else {
            auto car_front = cars_on_highway_.front();
            if (car_front == zone_obj_id) {
              cars_on_highway_.pop_front();      
            } else {
              SaveLog(zone_obj_id);
              ++failure_cnt_;
              while (car_front != zone_obj_id) {
                cars_on_highway_.pop_front();
                car_front = cars_on_highway_.front();
              }
              cars_on_highway_.pop_front();
            }
          }
        } else if (zone_event.type() == sensr_proto::ZoneEvent_Type_EXIT) {
          std::cout << "Exiting Zone(" << zone_event.id() << ") : obj( " << zone_event.object().id() << ")" << std::endl;
        }
      }
    }
  }
private:
  sensr::Client* client_;
  std::deque<int> cars_on_highway_;
  std::ofstream save_log_;
  int zone_entrance_;
  int zone_exit_;
  int failure_cnt_;
  std::deque<std::pair<std::string, std::deque<int>>> car_logs_;
  sys_clock::time_point prev_time_;
};

class PointResultListener : public sensr::MessageListener {
public:
  PointResultListener(sensr::Client* client) : MessageListener(ListeningType::kPointResult), client_(client) {}
  void OnError(Error error, const std::string& reason) {
    (void)reason;
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
    (void)reason;
    if (error == sensr::MessageListener::Error::kOutputMessageConnection || 
      error == sensr::MessageListener::Error::kPointResultConnection ) {
      client_->Reconnect();
    }
  }
  void OnGetOutputMessage(const sensr_proto::OutputMessage &message) {
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
    (void)reason;
    if (error == sensr::MessageListener::Error::kOutputMessageConnection || 
      error == sensr::MessageListener::Error::kPointResultConnection ) {
      client_->Reconnect();
    }
  }
  void OnGetOutputMessage(const sensr_proto::OutputMessage &message) {
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
  void OnError(Error error, const std::string& reason) {
    (void)reason;
    if (error == sensr::MessageListener::Error::kOutputMessageConnection || 
      error == sensr::MessageListener::Error::kPointResultConnection ) {
      client_->Reconnect();
    }
  }
  void OnGetOutputMessage(const sensr_proto::OutputMessage &message) {
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
  std::shared_ptr<sensr::MessageListener> listener;
  // Add sample listeners
  if (argc > 2) {
    for (int i = 2; i < argc; ++i) {
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
    listener = std::make_shared<ZoneEventListener>(&client);
    client.SubscribeMessageListener(listener);
  }
  std::string s;
  auto listener_ptr = std::dynamic_pointer_cast<ZoneEventListener>(listener);
  if (listener_ptr) {
    std::cout<<"Enter zone entrance id :";
    std::getline(std::cin, s);
    int entrance = std::stoi(s);
    std::cout<<"Enter zone exit id :";
    std::getline(std::cin, s);
    int exit = std::stoi(s);
    listener_ptr->SetZone(entrance, exit);
    std::cout<<"Type 'p' to print cars. Type 'exit' to exit."<<std::endl;
  }

  std::getline(std::cin, s);
  while(s != "exit") { // if the person type "exit", leave the loop
    if (s == "p" && listener_ptr) {
      listener_ptr->PrintElements();
    }
    std::getline(std::cin, s);
  }
  return 0;
}