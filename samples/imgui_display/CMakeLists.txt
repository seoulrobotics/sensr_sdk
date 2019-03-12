cmake_minimum_required(VERSION 3.8)
project(argos_sample_imgui VERSION 1.0.0 LANGUAGES CXX)


find_package(Protobuf REQUIRED)
find_package(ZeroMQ REQUIRED)
find_package(argos_sdk REQUIRED)
find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)

add_executable(imgui_display_sample main.cpp
              imgui/imgui.cpp
              imgui/imgui_widgets.cpp
              imgui/examples/imgui_impl_glfw.cpp
              imgui/examples/imgui_impl_opengl3.cpp
              imgui/imgui_draw.cpp
              glad/src/glad.cpp
)

target_compile_features(imgui_display_sample PRIVATE cxx_std_11)
target_compile_definitions(imgui_display_sample PRIVATE -DIMGUI_IMPL_OPENGL_LOADER_GLAD)
target_link_libraries(imgui_display_sample argos_sdk glfw ${CMAKE_DL_LIBS})
target_include_directories(imgui_display_sample
    PRIVATE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/imgui>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/glad/include>
)
