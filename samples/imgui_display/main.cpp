#include "imgui.h"
#include "examples/imgui_impl_glfw.h"
#include "examples/imgui_impl_opengl3.h"
#include <stdio.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "sensr.h"
#include <mutex>


class OutputMessageListener : public sensr::MessageListener {
public:
  OutputMessageListener(sensr::Client* client) : MessageListener(ListeningType::kOutputMessage), client_(client) {}
  void OnError(Error error, const std::string& reason) {
    if (error == sensr::MessageListener::Error::kOutputMessageConnection || 
      error == sensr::MessageListener::Error::kPointResultConnection ) {
      client_->Reconnect();
    }
  }
  void OnGetOutpuMessage(const sensr_proto::OutputMessage &message) {
    std::lock_guard<std::mutex> lock(mutex_);
    latest_message_ = message;
  }
  void ReceiveMessage(sensr_proto::OutputMessage & output) {
    std::lock_guard<std::mutex> lock(mutex_);
    output = latest_message_;
  }
private:
  sensr::Client* client_;
  std::mutex mutex_;
  sensr_proto::OutputMessage latest_message_;
};

const char *label_to_string[sensr_proto::LabelType::LABEL_MAX] =
    {
        "None",
        "Car",
        "Pedestrian",
        "Cyclist",
        "Misc",
        "Ground"
    };

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

  sensr_proto::OutputMessage output_msg;
  sensr_proto::StreamMessage latest_message;
  std::shared_ptr<OutputMessageListener> listener = std::make_shared<OutputMessageListener>(&client);
  client.SubscribeMessageListener(listener);
  int message_received_count = 0;
  while (!glfwWindowShouldClose(window))
  {
    glfwPollEvents();

    // Start the Dear ImGui frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    listener->ReceiveMessage(output_msg);
    if (output_msg.has_stream()) {
      latest_message = output_msg.stream();
      message_received_count++;
      
      const float kTrackedObjThreshold = 0.5f;
      const int size_of_vec3 = sizeof(float) * 3;
      size_t object_points_size = 0;
      int tracked_objects_size = 0, non_tracked_objects_size = 0;
      for(const auto& object : latest_message.objects()) {
        object_points_size += (object.points().length() / size_of_vec3);
        if (object.confidence() > kTrackedObjThreshold) {
          tracked_objects_size++;
        } else {
          non_tracked_objects_size++;
        }
      }

      ImGui::Begin("Message Explorer");

      ImGui::BulletText("Timestamp %ld (s) %d (ns)",
                        output_msg.timestamp().seconds(),
                        output_msg.timestamp().nanos());
      
      //ImGui::BulletText("Ground Points: %d.", latest_message.ground_points().length() / size_of_vec3);
      ImGui::BulletText("Object Points: %d.", object_points_size);
      ImGui::BulletText("Tracked Objects: %d.", tracked_objects_size);
      ImGui::BulletText("Non Tracked Objects: %d.", non_tracked_objects_size);

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
        for(const auto& object : latest_message.objects()) {
          if (object.confidence() <= kTrackedObjThreshold) {
            const Vector3& bbox_position = object.bbox().position();
            const Vector3& bbox_size = object.bbox().size();
            ImGui::Text("%d", object.id());
            ImGui::NextColumn();
            ImGui::Text("(%.3f, %.3f, %.3f) - (%.3f, %.3f, %.3f)",
                        bbox_position.x(), bbox_position.y(), bbox_position.z(),
                        bbox_size.x(), bbox_size.y(), bbox_size.z());
            ImGui::NextColumn();
            ImGui::Separator();
          }
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
        for(const auto& object : latest_message.objects()) {
          if (object.confidence() > kTrackedObjThreshold) {
            const Vector3& bbox_position = object.bbox().position();
            const Vector3& bbox_size = object.bbox().size();
            const Vector3& velocity = object.velocity();
            ImGui::Text("%d", object.id());
            ImGui::NextColumn();
            ImGui::Text("(%.3f, %.3f, %.3f) - (%.3f, %.3f, %.3f)",
                        bbox_position.x(), bbox_position.y(), bbox_position.z(),
                        bbox_size.x(), bbox_size.y(), bbox_size.z());
            ImGui::NextColumn();
            ImGui::Text("%s", label_to_string[(int)object.label()]);
            ImGui::NextColumn();
            ImGui::Text("%.3f", object.confidence());
            ImGui::NextColumn();
            ImGui::Text("none");
            ImGui::NextColumn();
            ImGui::Text("(%.3f, %.3f, %.3f )", velocity.x(), velocity.y(), velocity.z());
            ImGui::NextColumn();
            ImGui::Text("%d points", object.history().positions().size());
            ImGui::NextColumn();
            ImGui::Text("none");
            ImGui::NextColumn();
            ImGui::Separator();
          }
        }

        ImGui::Columns(1);

        ImGui::EndTabItem();
      }

      ImGui::EndTabBar();
      ImGui::End();
    }

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
