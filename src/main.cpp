#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <OpenGL/gl.h>
#include <spdlog/common.h>
#include <spdlog/spdlog.h>

#include <vector>

#define log spdlog::info
#define errorLog spdlog::error

void DebugEnv();

void CreateTriangle();

int main() {
  GLFWwindow* window;

  if (!glfwInit()) {
    errorLog("Fail to Init GLFW");
    return -1;
  }

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  window = glfwCreateWindow(800, 600, "Hello opengl", NULL, NULL);
  if (!window) {
    glfwTerminate();
    errorLog("Fail to Create Window");
    return -1;
  }

  glfwMakeContextCurrent(window);

  DebugEnv();

  while (!glfwWindowShouldClose(window)) {
    glClear(GL_COLOR_BUFFER_BIT);

    glfwSwapBuffers(window);

    glfwPollEvents();
  }

  glfwTerminate();

  return 0;
}

void DebugEnv() {
  const char* glVersion = (const char*)glGetString(GL_VERSION);
  const char* glRenderer = (const char*)glGetString(GL_RENDERER);
  const char* glVendor = (const char*)glGetString(GL_VENDOR);

  log("OpenGL Version : {}", glVersion);
  log("OpenGL Model : {}", glRenderer);
  log("OpenGL Vendor : {}", glVendor);
}

void CreateTriangle() {
  // clang-format off
  std::vector<float> buffers = {
      -0.5f, -0.5f, 0.0f, // left
      0.5f, -0.5f, 0.0f, // right
      0.0f, 0.5f, 0.0f, // top
  };
  // clang-format on

  unsigned int VBO;
  glGenBuffers(1, &VBO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, buffers.size() * sizeof(float), buffers.data(), GL_STATIC_DRAW);
}
