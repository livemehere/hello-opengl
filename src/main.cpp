#include <GLFW/glfw3.h>
#include <iostream>

void error_callback(int error, const char *desc) {
  std::cout << error << "Error : " << desc << std::endl;
}

int main() {
  GLFWwindow *window;

  glfwSetErrorCallback(error_callback);

  if (!glfwInit()) {
    std::cout << "Could not start GLFW" << std::endl;
    return -1;
  }

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  // glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

  window = glfwCreateWindow(1280, 720, "Hello OpenGl", NULL, NULL);

  glfwMakeContextCurrent(window); // 1. creat OpenGL context

  while (!glfwWindowShouldClose(window)) {

    glClear(GL_COLOR_BUFFER_BIT);

    glfwSwapBuffers(window);

    glfwPollEvents();
  }

  glfwTerminate();

  return 0;
}
