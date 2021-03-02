set(Protobuf_USE_STATIC_LIBS ON)

find_package(Protobuf REQUIRED)

set(PROTOBUF_GENERATE_CPP_APPEND_PATH OFF)
file(GLOB_RECURSE PROTO_FILES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "*.proto")
protobuf_generate_cpp(generated_code generated_header_code ${PROTO_FILES})
add_library(sensr_proto STATIC ${generated_code} ${generated_header_code})
target_link_libraries(sensr_proto ${Protobuf_LIBRARIES})
target_include_directories(sensr_proto
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
    PRIVATE
        $<BUILD_INTERFACE:${Protobuf_INCLUDE_DIR}>
)
target_compile_options(sensr_proto PRIVATE -fPIC)
set_target_properties(sensr_proto PROPERTIES PUBLIC_HEADER "${generated_header_code}")

install(
    TARGETS sensr_proto
    EXPORT "${TARGETS_EXPORT_NAME}"
    LIBRARY DESTINATION "${CMAKE_INSTALL_LIBDIR}"
    ARCHIVE DESTINATION "${CMAKE_INSTALL_LIBDIR}"
    RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}"
    INCLUDES DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/sensr_proto"
    PUBLIC_HEADER DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/sensr_proto"
)
