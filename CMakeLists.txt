cmake_minimum_required(VERSION 3.8)
project(sensr_sdk VERSION 1.0.0 LANGUAGES CXX)

######## PACKAGE Dependency Management ##########
find_package(OpenSSL REQUIRED)
######## PACKAGE Dependency Management ##########

option(ENABLE_INFO_LOG "enable lnfo level log message"  OFF)
option(ENABLE_ERROR_LOG "enable error level log message"  ON)

if(${ENABLE_INFO_LOG})
    add_definitions(-DINFO_LOG_ENABLE=1)
else()
    add_definitions(-DINFO_LOG_ENABLE=0)
endif()

if(${ENABLE_ERROR_LOG})
    add_definitions(-DERROR_LOG_ENABLE=1)
else()
    add_definitions(-DERROR_LOG_ENABLE=0)
endif()

add_definitions(-DASIO_STANDALONE)

set(SOURCE_FILES
    src/sensr_client.cpp
    src/sensr_message_listener.cpp
    src/message_broker.cpp
    src/websocket/websocket_endpoint_base.cpp
    src/websocket/websocket_endpoint.cpp
    src/websocket/websocket_secure_endpoint.cpp)
add_library(sensr_sdk SHARED ${generated_code} ${generated_header_code} ${SOURCE_FILES})

####### Compiler Flags ###########
target_compile_features(sensr_sdk PRIVATE cxx_std_11)
target_compile_options(sensr_sdk PRIVATE -fPIC -Wall -Werror -rdynamic)
target_compile_options(sensr_sdk PRIVATE -Wno-attributes -Wno-deprecated -Wno-comment -Wno-strict-aliasing -Wno-format-security -Wno-int-to-pointer-cast -Wno-unused-result -Wno-unused-function)
####### Compiler Flags ###########

target_include_directories(sensr_sdk
    PUBLIC
        $<INSTALL_INTERFACE:include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/external/websocketpp>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/external/asio/asio/include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
)

target_link_libraries(sensr_sdk PUBLIC  sensr_proto
                                        ${OPENSSL_LIBRARIES}
                                PRIVATE pthread)

######## Installation script ##########

include(CMakePackageConfigHelpers)
include(GNUInstallDirs)

set(generated_dir "${CMAKE_CURRENT_BINARY_DIR}/generated")
set(config_install_dir "${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}")
set(version_config "${generated_dir}/${PROJECT_NAME}ConfigVersion.cmake")
set(project_config "${generated_dir}/${PROJECT_NAME}Config.cmake")
set(TARGETS_EXPORT_NAME "${PROJECT_NAME}Targets")
set(namespace "")

file(GLOB_RECURSE header_files
  "${PROJECT_SOURCE_DIR}/include/*.h"
)

add_subdirectory(proto)

set_target_properties(sensr_sdk PROPERTIES PUBLIC_HEADER "${header_files}")

write_basic_package_version_file(
    "${version_config}" COMPATIBILITY SameMajorVersion
)

configure_package_config_file(
    "cmake/Config.cmake.in"
    "${project_config}"
    INSTALL_DESTINATION "${config_install_dir}"
)

install(
    TARGETS sensr_sdk
    EXPORT "${TARGETS_EXPORT_NAME}"
    LIBRARY DESTINATION "${CMAKE_INSTALL_LIBDIR}"
    ARCHIVE DESTINATION "${CMAKE_INSTALL_LIBDIR}"
    RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}"
    INCLUDES DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}"
    PUBLIC_HEADER DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}"
)

install(
    FILES "${project_config}" "${version_config}"
    DESTINATION "${config_install_dir}"
)

install(
    EXPORT "${TARGETS_EXPORT_NAME}"
    NAMESPACE "${namespace}"
    DESTINATION "${config_install_dir}"
)

######## Installation script ##########


