#include <GLFW/glfw3.h>
#include <OpenGL/gl.h>
#include <spdlog/common.h>
#include <spdlog/spdlog.h>

#define log spdlog::info
#define errorLog spdlog::error

int main() {

  GLFWwindow *window;

  if (!glfwInit()) {
    errorLog("Fail to Init GLFW");
    return -1;
  }

  window = glfwCreateWindow(800, 600, "Hello opengl", NULL, NULL);
  if (!window) {
    glfwTerminate();
    errorLog("Fail to Create Window");
    return -1;
  }

  glfwMakeContextCurrent(window);

  while (!glfwWindowShouldClose(window)) {

    glClear(GL_COLOR_BUFFER_BIT);

    glfwSwapBuffers(window);

    glfwPollEvents();
  }

  glfwTerminate();

  return 0;
}
