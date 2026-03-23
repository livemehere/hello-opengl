#include <GLFW/glfw3.h>
#include <OpenGL/gl.h>
#include <iostream>

void error_callback(int error, const char *desc) {
  std::cout << error << "Error : " << desc << std::endl;
}

void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
  glViewport(0, 0, width, height);
}

void handleInput(GLFWwindow *window) {
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
    glfwSetWindowShouldClose(window, true);
  }
}

constexpr int width = 1280;
constexpr int height = 720;

int main() {
  GLFWwindow *window;

  glfwSetErrorCallback(error_callback);

  // 리사이징 될때마다 viewPort 재정의 필요
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

  if (!glfwInit()) {
    std::cout << "Could not start GLFW" << std::endl;
    return -1;
  }

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

  // 모던하게 가겠다.
  // glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  // 맥에서는 아래도, 모던하게 가겠다는 의미.
#ifdef APPLE
// glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

  window = glfwCreateWindow(width, height, "Hello OpenGl", NULL, NULL);

  glfwMakeContextCurrent(window); // 1. creat OpenGL context

  glViewport(0, 0, width, height);

  while (!glfwWindowShouldClose(window)) {

    handleInput(window);

    glClear(GL_COLOR_BUFFER_BIT);

    glfwSwapBuffers(window);

    glfwPollEvents();
  }

  // 모든 리소스 정리
  glfwTerminate();

  return 0;
}
