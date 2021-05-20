cmake_minimum_required(VERSION 3.8)
project(sensr_output_recorder VERSION 1.0.0 LANGUAGES CXX)

find_package(sensr_sdk REQUIRED)

set(Protobuf_USE_STATIC_LIBS ON)

find_package(Protobuf REQUIRED)

set(PROTOBUF_GENERATE_CPP_APPEND_PATH OFF)
set(PROTOBUF_IMPORT_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/../../proto)
message (STATUS "PROTOBUF_IMPORT_DIRS       " ${PROTOBUF_IMPORT_DIRS})
file(GLOB PROTO_FILES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "*.proto")
protobuf_generate_cpp(generated_code generated_header_code ${PROTO_FILES})
add_library(replay_proto STATIC ${generated_code} ${generated_header_code})
target_link_libraries(replay_proto ${Protobuf_LIBRARIES})
target_include_directories(replay_proto
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
    PRIVATE
        $<BUILD_INTERFACE:${Protobuf_INCLUDE_DIR}>
)
target_compile_options(replay_proto PRIVATE -fPIC)
set_target_properties(replay_proto PROPERTIES PUBLIC_HEADER "${generated_header_code}")

add_executable(sensr_output_recorder main.cpp)
target_link_libraries(sensr_output_recorder sensr_sdk sensr_proto replay_proto)
target_include_directories(sensr_output_recorder PRIVATE sensr_sdk::INCLUDE_DIRECTORIES)

