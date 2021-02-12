cmake_minimum_required(VERSION 3.8)
project(sensr_output_recorder VERSION 1.0.0 LANGUAGES CXX)

find_package(Protobuf REQUIRED)
find_package(sensr_sdk REQUIRED)

add_executable(sensr_output_recorder main.cpp)
target_link_libraries(sensr_output_recorder sensr_sdk sensr_proto)
target_include_directories(sensr_output_recorder PRIVATE sensr_sdk::INCLUDE_DIRECTORIES)

