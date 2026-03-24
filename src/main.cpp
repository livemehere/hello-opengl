// #define GLFW_INCLUDE_GLCOREARB
#include <GLFW/glfw3.h>
#include <iostream>

// 논리적인 사이즈
constexpr int w = 800;
constexpr int h = 600;

void OnError(int error, const char *desc) {
  std::cout << error << desc << std::endl;
}

void HandleKey(GLFWwindow *window, int key, int scancoode, int action,
               int mods) {
  std::cout << key << " " << scancoode << " " << action << " " << mods
            << std::endl;

  if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
    glfwSetWindowShouldClose(window, GLFW_TRUE);
  }
}

void SetViewSize(GLFWwindow *window) {
  int w, h;
  glfwGetFramebufferSize(window, &w, &h);
  glViewport(0, 0, w, h);
}

int main() {

  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE,
                 GLFW_OPENGL_CORE_PROFILE); // set use only modern functions
                                            // (disable legacy)

  // TODO: 성틍 및 결과 테스트 해보기
  // #ifdef __APPLE__
  //   glfwWindowHint(GLFW_COCOA_RETINA_FRAMEBUFFER, GLFW_FALSE);
  // #endif

  // 1.init
  glfwSetErrorCallback(OnError);
  if (!glfwInit()) {
    std::cout << "GLFW init fail" << std::endl;
    glfwTerminate();
    return -1;
  }

  // 2.create window (fail check)
  GLFWwindow *window = glfwCreateWindow(w, h, "Hello OpenGL", NULL, NULL);
  if (!window) {
    std::cout << "Error while Create GLFW window" << std::endl;
    glfwTerminate();
    return -1;
  }
  SetViewSize(window);
  glfwMakeContextCurrent(window);
  // ---

  // 3.loop
  glfwSetKeyCallback(window, HandleKey);
  while (!glfwWindowShouldClose(window)) {
    // draw
    glfwPollEvents();
  }
  glfwDestroyWindow(window);
  glfwTerminate();

  return 0;
}
