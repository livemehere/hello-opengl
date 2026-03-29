
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <OpenGL/gl3.h>

#include <vector>

#include "Engine/Debug.hpp"
#include "Engine/IndexBuffer.hpp"
#include "Engine/Renderer.hpp"
#include "Engine/Shader.hpp"
#include "Engine/Utils.hpp"
#include "Engine/VertexArray.hpp"
#include "Engine/VertexBuffer.hpp"
#include "Engine/VertexBufferyLayout.hpp"

struct Output {
  unsigned int program;
  unsigned int VAO;
  VertexBuffer vb;
  IndexBuffer ib;
  int totalPoints;
  int totalIndicies;
};

void DebugEnv() {
  const char* glVersion = (const char*)glGetString(GL_VERSION);
  const char* glRenderer = (const char*)glGetString(GL_RENDERER);
  const char* glVendor = (const char*)glGetString(GL_VENDOR);

  LOG("OpenGL Version : {}", glVersion);
  LOG("OpenGL Model : {}", glRenderer);
  LOG("OpenGL Vendor : {}\n", glVendor);
}

int main() {
  GLFWwindow* window;

  if (!glfwInit()) {
    ERROR_LOG("Fail to Init GLFW");
    return -1;
  }

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  window = glfwCreateWindow(800, 600, "Hello opengl", NULL, NULL);
  if (!window) {
    glfwTerminate();
    ERROR_LOG("Fail to Create Window");
    return -1;
  }

  glfwMakeContextCurrent(window);
  glfwSwapInterval(1);

  DebugEnv();

  {
    // create
    Shader shader("../assets/shaders/basic.vs", "../assets/shaders/basic.fs");

    // clang-format off
  std::vector<float> buffers = {
      -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f,// bl
      0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, // br
      0.5f, 0.5f, 0.0f,0.0f, 0.0f, 1.0f, // tr

      -0.5f, 0.5f, 0.0f,0.0f, 0.0f, 1.0f // tl
  };
  std::vector<unsigned int> indicies = {
    0, 1, 2,
    0, 2, 3
  };
    // clang-format on

    VertexArray va;
    VertexBuffer vb(buffers.data(), buffers.size() * sizeof(float));
    IndexBuffer ib(indicies.data(), indicies.size());

    VertexBufferLayout layout;
    layout.Push<float>(3);
    layout.Push<float>(3);
    va.AddBuffer(vb, layout);

    va.UnBind();
    vb.UnBind();
    ib.UnBind();
    shader.UnBind();

    Renderer renderer;

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    while (!glfwWindowShouldClose(window)) {
      renderer.Clear();

      float time = glfwGetTime();
      shader.Bind();
      float r = (sin(time) + 1.0f) / 2.0f;
      float g = (sin(time + 2.0f) + 1.0f) / 2.0f;
      float b = (sin(time + 3.0f) + 1.0f) / 2.0f;
      shader.SetUniform4f("u_color", r, g, b, 1.0f);

      renderer.Draw(va, ib);

      glfwSwapBuffers(window);

      glfwPollEvents();
    }
  }

  glfwTerminate();

  return 0;
}
