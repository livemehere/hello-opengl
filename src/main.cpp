#include <GLFW/glfw3.h>
#include <spdlog/spdlog.h>

int main() {
  GLFWwindow *window;

  spdlog::info("Hello, world!");
  if (!glfwInit()) {
    spdlog::info("Fail to Init GLFW");
    return -1;
  }

  return 0;
}
