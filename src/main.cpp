#include <GLFW/glfw3.h>
#include <iostream>

void error_callback(int error, const char *desc) {
  std::cout << error << "Error : " << desc << std::endl;
}

int main() {
  GLFWwindow *window;

  glfwSetErrorCallback(error_callback);

  if (!glfwInit()) {
    return -1;
  }

  window = glfwCreateWindow(1280, 720, "Hello OpenGl", NULL, NULL);

  glfwMakeContextCurrent(window);

  while (!glfwWindowShouldClose(window)) {

    glClear(GL_COLOR_BUFFER_BIT);

    glfwSwapBuffers(window);

    glfwPollEvents();
  }

  glfwTerminate();

  return 0;
}
