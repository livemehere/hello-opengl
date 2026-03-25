#include <OpenGL/gl.h>
#define GLFW_INCLUDE_GLCOREARB
#include "Shader.hpp"
#include <GLFW/glfw3.h>
#include <iostream>

#define STB_IMAGE_IMPLEMENTATION
#include "../external/include/stb_image.h"

// 논리적인 사이즈
constexpr int w = 800;
constexpr int h = 600;

std::string vsSrc = R"(
  #version 330 core
  layout (location = 0) in vec3 aPos;
  
  void main()
  {
    gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0f);
  }
)";

std::string fsCrc = R"(
  #version 330 core
  out vec4 FragColor;

  void main()
  {
    FragColor = vec4(1.0f, 0.9f, 0.2f, 0.1f);
  }
)";

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
  glfwMakeContextCurrent(window);
  SetViewSize(window);

  Shader triangle(vsSrc, fsCrc,
                  {
                      // bottom-left
                      -0.5f,
                      -0.5f,
                      0.0f,
                      //
                      0.5f,
                      -0.5f,
                      0.0f,
                      //
                      -0.5f,
                      0.5f,
                      0.0f,
                      // top-right
                      // -0.5f,
                      // 0.5f,
                      // 0.0f,
                      //
                      0.5f,
                      0.5f,
                      0.0f,
                      //
                      // 0.5f,
                      // -0.5f,
                      // 0.0f,

                  },
                  // indices
                  {0, 1, 2, 1, 2, 3}

  );

  int imgW, imgH, numColCh;
  unsigned char *bytes =
      stbi_load("../assets/wall.png", &imgW, &imgH, &numColCh, 0);

  // 3.loop
  glfwSetKeyCallback(window, HandleKey);

  // alpha 사용하도록 (기존 비활성화임)
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  while (!glfwWindowShouldClose(window)) {

    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    // draw
    triangle.Use();
    glfwSwapBuffers(window);

    glfwPollEvents();
  }
  glfwDestroyWindow(window);
  glfwTerminate();

  return 0;
}
