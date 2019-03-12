cmake_minimum_required(VERSION 3.8)
project(argos_console_sample VERSION 1.0.0 LANGUAGES CXX)

find_package(Protobuf REQUIRED)
find_package(ZeroMQ REQUIRED)
find_package(argos_sdk REQUIRED)

add_executable(console_output_sample main.cpp)
target_link_libraries(console_output_sample argos_sdk)
target_include_directories(console_output_sample PRIVATE argos_sdk::INCLUDE_DIRECTORIES)

