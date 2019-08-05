#include "imgui.h"
#include "examples/imgui_impl_glfw.h"
#include "examples/imgui_impl_opengl3.h"
#include <stdio.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "sensr.h"

const char *label_to_string[sensr::LabelType::Max+1] =
    {
        "Car",
        "Pedestrian",
        "Cyclist",
        "Misc",
        "INVALID"};

int main(int argc, char *argv[])
{
  const char *client_address = "localhost";
  if (argc > 1)
  {
    client_address = argv[1];
  }

  sensr::Client client(client_address);

  if (!glfwInit())
    return 1;

    // Decide GL+GLSL versions
#if __APPLE__
  // GL 3.2 + GLSL 150
  const char *glsl_version = "#version 150";
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // 3.2+ only
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);           // Required on Mac
#else
  // GL 3.0 + GLSL 130
  const char *glsl_version = "#version 130";
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
#endif

  // Create window with graphics context
  GLFWwindow *window = glfwCreateWindow(1280, 720, "Seoul Robotics SDK Imgui Sample", NULL, NULL);
  if (window == NULL)
    return 1;
  glfwMakeContextCurrent(window);
  glfwSwapInterval(1); // Enable vsync

  // Initialize OpenGL loader
  bool err = gladLoadGL() == 0;
  if (err)
  {
    fprintf(stderr, "Failed to initialize OpenGL loader!\n");
    return 1;
  }

  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO &io = ImGui::GetIO();
  (void)io;
  ImGui::StyleColorsDark();
  ImGui_ImplGlfw_InitForOpenGL(window, true);
  ImGui_ImplOpenGL3_Init(glsl_version);

  output_message latest_message;
  int message_received_count = 0;
  while (!glfwWindowShouldClose(window))
  {
    glfwPollEvents();

    // Start the Dear ImGui frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    if (client.ReceiveMessageAsync(latest_message))
    {
      message_received_count++;
    }

    ImGui::Begin("Message Explorer");

    ImGui::BulletText("Timestamp %ld (s) %d (ns)",
                      latest_message.time_stamp().seconds(),
                      latest_message.time_stamp().nano_seconds());

    ImGui::BulletText("Ground Points: %d.", latest_message.point_cloud().ground_points().size());
    ImGui::BulletText("Object Points: %d.", latest_message.point_cloud().object_points().size());
    ImGui::BulletText("Invalid Points: %d.", latest_message.point_cloud().invalid_points().size());
    ImGui::BulletText("Tracked Objects: %d.", latest_message.tracked_objects_size());
    ImGui::BulletText("Non Tracked Objects: %d.", latest_message.non_tracked_objects_size());

    ImGui::BeginTabBar("Test");

    if (ImGui::BeginTabItem("Non Tracked Objects"))
    {

      ImGui::Columns(2);
      ImGui::Separator();
      ImGui::Text("Id");
      ImGui::NextColumn();
      ImGui::Text("Bounding Box");
      ImGui::NextColumn();
      ImGui::Separator();

      for (int i = 0; i < latest_message.non_tracked_objects_size(); ++i)
      {
        auto &item = latest_message.non_tracked_objects(i);
        const float *bbox_position = (const float *)item.bbox().position().data();
        const float *bbox_size = (const float *)item.bbox().size().data();
        ImGui::Text("%d", item.id());
        ImGui::NextColumn();
        ImGui::Text("(%.3f, %.3f, %.3f) - (%.3f, %.3f, %.3f)",
                    bbox_position[0], bbox_position[1], bbox_position[2],
                    bbox_size[0], bbox_size[1], bbox_size[2]);
        ImGui::NextColumn();
        ImGui::Separator();
      }

      ImGui::Columns(1);

      ImGui::EndTabItem();
    }

    if (ImGui::BeginTabItem("Tracked Objects"))
    {
      ImGui::Columns(8);
      ImGui::Separator();
      ImGui::Text("Id");
      ImGui::NextColumn();
      ImGui::Text("Bounding Box");
      ImGui::NextColumn();
      ImGui::Text("Label");
      ImGui::NextColumn();
      ImGui::Text("Probability");
      ImGui::NextColumn();
      ImGui::Text("Tracked");
      ImGui::NextColumn();
      ImGui::Text("Velocity");
      ImGui::NextColumn();
      ImGui::Text("History Points");
      ImGui::NextColumn();
      ImGui::Text("Prediction Points");
      ImGui::NextColumn();
      ImGui::Separator();

      for (int i = 0; i < latest_message.tracked_objects_size(); ++i)
      {
        auto &item = latest_message.tracked_objects(i);
        const float *bbox_position = (const float *)item.bbox().position().data();
        const float *bbox_size = (const float *)item.bbox().size().data();
        const float *velocity = (const float *)item.velocity().data();
        ImGui::Text("%d", item.id());
        ImGui::NextColumn();
        ImGui::Text("(%.3f, %.3f, %.3f) - (%.3f, %.3f, %.3f)",
                    bbox_position[0], bbox_position[1], bbox_position[2],
                    bbox_size[0], bbox_size[1], bbox_size[2]);
        ImGui::NextColumn();
        ImGui::Text("%s", label_to_string[item.label()]);
        ImGui::NextColumn();
        ImGui::Text("%.3f", item.probability());
        ImGui::NextColumn();
        ImGui::Text(item.tracking_reliable() ? "Reliable" : "Unreliable");
        ImGui::NextColumn();
        ImGui::Text("(%.3f, %.3f, %.3f )", velocity[0], velocity[1], velocity[2]);
        ImGui::NextColumn();
        ImGui::Text("%d points", item.history().size());
        ImGui::NextColumn();
        ImGui::Text("%d points", item.prediction().size());
        ImGui::NextColumn();
        ImGui::Separator();
      }

      ImGui::Columns(1);

      ImGui::EndTabItem();
    }

    ImGui::EndTabBar();
    ImGui::End();

    // Rendering
    ImGui::Render();
    int display_w, display_h;
    glfwMakeContextCurrent(window);
    glfwGetFramebufferSize(window, &display_w, &display_h);
    glViewport(0, 0, display_w, display_h);
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    glfwMakeContextCurrent(window);
    glfwSwapBuffers(window);
  }

  // Cleanup
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();

  glfwDestroyWindow(window);
  glfwTerminate();

  return 0;
}
