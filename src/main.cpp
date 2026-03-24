#include <GLFW/glfw3.h>
#include <OpenGL/gl.h>
#include <iostream>

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

int main() {

  // glfw init
  glfwInit();
  // set opengl version(major, minor)
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  // set use only modern functions (disable legacy)
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  glfwSetErrorCallback(OnError);

  if (!glfwInit()) {
    std::cout << "GLFW init fail" << std::endl;
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
  glViewport(0, 0, w, h);
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
