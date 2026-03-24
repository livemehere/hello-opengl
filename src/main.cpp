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

  // glfw init
  glfwInit();
  // set opengl version(major, minor)
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  // set use only modern functions (disable legacy)
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  // TODO: 성틍 및 결과 테스트 해보기
  // #ifdef __APPLE__
  //   glfwWindowHint(GLFW_COCOA_RETINA_FRAMEBUFFER, GLFW_FALSE);
  // #endif

  glfwSetErrorCallback(OnError);

  if (!glfwInit()) {
    std::cout << "GLFW init fail" << std::endl;
    glfwTerminate();
    return -1;
  }

  // create window (fail check)
  GLFWwindow *window = glfwCreateWindow(w, h, "Hello OpenGL", NULL, NULL);
  if (!window) {
    std::cout << "Error while Create GLFW window" << std::endl;
    glfwTerminate();
    return -1;
  }

  glfwSetKeyCallback(window, HandleKey);

  // create context (gl)
  glfwMakeContextCurrent(window);
  // load glad (skip on macos)
  // set viewport
  SetViewSize(window);
  // while loop
  while (!glfwWindowShouldClose(window)) {
    // draw
    glfwPollEvents();
  }
  // destory window
  glfwDestroyWindow(window);
  // terminate glfw
  glfwTerminate();

  return 0;
}
