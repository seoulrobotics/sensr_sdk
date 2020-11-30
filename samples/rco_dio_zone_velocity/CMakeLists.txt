cmake_minimum_required(VERSION 3.8)
project(sensr_console_sample VERSION 1.0.0 LANGUAGES CXX)

find_package(Protobuf REQUIRED)
find_package(ZeroMQ REQUIRED)
find_package(sensr_sdk REQUIRED)

add_executable(rco_dio_zone_velocity main.cpp)
target_link_libraries(rco_dio_zone_velocity sensr_sdk)
target_include_directories(rco_dio_zone_velocity PRIVATE sensr_sdk::INCLUDE_DIRECTORIES)

