#include <GLFW/glfw3.h>
#include <OpenGL/gl.h>
#include <OpenGL/gl3.h>
#include <iostream>

void error_callback(int error, const char *desc) {
  std::cout << error << "Error : " << desc << std::endl;
}

void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
  int bufferWidth, bufferHeight;
  glfwGetFramebufferSize(window, &bufferWidth, &bufferHeight);
  glViewport(0, 0, bufferWidth, bufferHeight);
  std::cout << "resize!!" << std::endl;
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

  if (!glfwInit()) {
    std::cout << "Could not start GLFW" << std::endl;
    return -1;
  }

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

  // 모던하게 가겠다.
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  // 맥에서는 아래도, 모던하게 가겠다는 의미.
#ifdef __APPLE__
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

  window = glfwCreateWindow(width, height, "Hello OpenGl", NULL, NULL);

  // 리사이징 될때마다 viewPort 재정의 필요
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

  glfwMakeContextCurrent(window); // 1. creat OpenGL context

  // 레티나 디스플레이는 실제 픽셀 수가 더 많기 때문에, 버퍼 사이즈를 가져와서
  // 뷰포트를 설정해준다.
  int bufferWidth, bufferHeight;
  glfwGetFramebufferSize(window, &bufferWidth, &bufferHeight);
  glViewport(0, 0, bufferWidth, bufferHeight);

  // ========================== start ==========================

  // 4. vertex shader 작성
  const char *vertexShaderSrc =
      "#version 330 core\n"
      "layout (location = 0) in vec3 aPos;\n"
      "void main()\n"
      "{\n"
      "gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
      "}\n";

  // 5. vertex shader compile
  unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertexShader, 1, &vertexShaderSrc, NULL);
  glCompileShader(vertexShader);

  // 6. Fragment shader
  const char *fragmentShaderSrc = "#version 330 core\n"
                                  "out vec4 FragColor;\n"
                                  "void main()\n"
                                  "{\n"
                                  "FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0);\n"
                                  "}\n";

  unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragmentShader, 1, &fragmentShaderSrc, NULL);
  glCompileShader(fragmentShader);

  // 7. shader program
  unsigned int shaderProgram = glCreateProgram();
  glAttachShader(shaderProgram, vertexShader);
  glAttachShader(shaderProgram, fragmentShader);
  glLinkProgram(shaderProgram);

  // program 에 합치고나서는 제거
  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);

  // vertex
  float vertices[] = {
      -0.5f, -0.5f, 0.0f, // left
      0.5f,  -0.5f, 0.0f, // right
      0.0f,  0.5f,  0.0f, // top
  };

  // buffer 객체 ID 생성
  unsigned int VBO, VAO;
  glGenVertexArrays(1, &VAO); // 설명서
  glGenBuffers(1, &VBO);      // 데이터 상자

  glBindVertexArray(VAO);

  // vertex buffer 타입으로 지정
  glBindBuffer(GL_ARRAY_BUFFER, VBO);

  // gpu buffer 에 데이터 복사
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  // 데이터 해석방법 설정 (0번 속성은 float 3개가 1세트이다)
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0); // 0번 속성 스위치 on

  // 다른곳에서 실수로 건들지 않게 해제
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);

  while (!glfwWindowShouldClose(window)) {

    handleInput(window);

    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(shaderProgram);
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 3);

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  // 모든 리소스 정리
  glfwTerminate();

  return 0;
}
