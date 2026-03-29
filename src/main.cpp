#include <OpenGL/gl.h>
#include <OpenGL/glext.h>
#include <OpenGL/gltypes.h>

#include <stdexcept>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <OpenGL/gl3.h>

#include <format>
#include <fstream>
#include <vector>

#include "Engine/Debug.hpp"
#include "Engine/IndexBuffer.hpp"
#include "Engine/VertexArray.hpp"
#include "Engine/VertexBuffer.hpp"
#include "Engine/VertexBufferyLayout.hpp"

struct Output {
  unsigned int program;
  unsigned int VAO;
  VertexBuffer vb;
  IndexBuffer ib;
  int totalPoints;
  int totalIndicies;
};

void DebugEnv() {
  const char* glVersion = (const char*)glGetString(GL_VERSION);
  const char* glRenderer = (const char*)glGetString(GL_RENDERER);
  const char* glVendor = (const char*)glGetString(GL_VENDOR);

  LOG("OpenGL Version : {}", glVersion);
  LOG("OpenGL Model : {}", glRenderer);
  LOG("OpenGL Vendor : {}\n", glVendor);
}

std::string ReadFile(std::string path) {
  std::ifstream file(path);
  if (!file.is_open()) {
    throw std::runtime_error(std::format("Can not Open the File : {} ", path));
  }

  std::stringstream buffer;
  buffer << file.rdbuf();
  return buffer.str();
}

unsigned int CompileShader(GLenum type, const std::string& shaderSource) {
  unsigned int shader;
  shader = glCreateShader(type);

  const char* src = shaderSource.c_str();
  glShaderSource(shader, 1, &src, NULL);
  glCompileShader(shader);

  std::string typeStr = type == GL_VERTEX_SHADER ? "Vertex" : "Fragment";
  int success;
  char infoLog[512];
  glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(shader, 512, NULL, infoLog);
    ERROR_LOG("{} Shader Compile Error : {}", typeStr, infoLog);
  } else {
    LOG("{} shader compile success", typeStr);
  }

  return shader;
}

unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader) {
  unsigned int program;
  program = glCreateProgram();

  unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
  unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

  glAttachShader(program, vs);
  glAttachShader(program, fs);
  glLinkProgram(program);

  int success;
  char infoLog[512];
  glGetProgramiv(program, GL_LINK_STATUS, &success);
  if (!success) {
    glGetProgramInfoLog(program, 512, NULL, infoLog);
    ERROR_LOG("Program Link Error : {}", infoLog);
  } else {
    LOG("program link success");
  }

  glDeleteShader(vs);
  glDeleteShader(fs);

  return program;
}

int main() {
  GLFWwindow* window;

  if (!glfwInit()) {
    ERROR_LOG("Fail to Init GLFW");
    return -1;
  }

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  window = glfwCreateWindow(800, 600, "Hello opengl", NULL, NULL);
  if (!window) {
    glfwTerminate();
    ERROR_LOG("Fail to Create Window");
    return -1;
  }

  glfwMakeContextCurrent(window);
  glfwSwapInterval(1);

  DebugEnv();

  // create
  std::string vertexSrc = ReadFile("../assets/shaders/basic.vs");
  std::string fragmentSrc = ReadFile("../assets/shaders/basic.fs");

  unsigned int program = CreateShader(vertexSrc, fragmentSrc);
  int colorLoc = glGetUniformLocation(program, "u_color");

  // clang-format off
  std::vector<float> buffers = {
      -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f,// bl
      0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, // br
      0.5f, 0.5f, 0.0f,0.0f, 0.0f, 1.0f, // tr

      -0.5f, 0.5f, 0.0f,0.0f, 0.0f, 1.0f // tl
  };
  std::vector<unsigned int> indicies = {
    0, 1, 2,
    0, 2, 3
  };
  // clang-format on

  VertexArray va;
  VertexBuffer vb(buffers.data(), buffers.size() * sizeof(float));
  IndexBuffer ib(indicies.data(), indicies.size());

  VertexBufferLayout layout;
  layout.Push<float>(3);
  layout.Push<float>(3);
  va.AddBuffer(vb, layout);

  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
  while (!glfwWindowShouldClose(window)) {
    glClear(GL_COLOR_BUFFER_BIT);

    float time = glfwGetTime();

    // use
    glUseProgram(program);

    float r = (sin(time) + 1.0f) / 2.0f;
    float g = (sin(time + 2.0f) + 1.0f) / 2.0f;
    float b = (sin(time + 3.0f) + 1.0f) / 2.0f;
    glUniform4f(colorLoc, r, g, b, 1.0f);

    va.Bind();
    ib.Bind();
    glDrawElements(GL_TRIANGLES, indicies.size(), GL_UNSIGNED_INT, NULL);
    va.UnBind();
    ib.UnBind();

    glfwSwapBuffers(window);

    glfwPollEvents();
  }

  glDeleteProgram(program);

  glfwTerminate();

  return 0;
}
