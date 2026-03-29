#include <OpenGL/gl.h>
#include <OpenGL/glext.h>
#include <OpenGL/gltypes.h>

#include <stdexcept>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <OpenGL/gl3.h>
#include <spdlog/common.h>
#include <spdlog/spdlog.h>

#include <format>
#include <fstream>
#include <vector>

#define log spdlog::info
#define errorLog spdlog::error

GLenum glCheckError_(const char* file, int line) {
  GLenum errorCode = GL_NO_ERROR;
  while ((errorCode = glGetError()) != GL_NO_ERROR) {
    std::string error;
    switch (errorCode) {
      case GL_INVALID_ENUM:
        error = "INVALID_ENUM";
        break;
      case GL_INVALID_VALUE:
        error = "INVALID_VALUE";
        break;
      case GL_INVALID_OPERATION:
        error = "INVALID_OPERATION";
        break;
      case GL_STACK_OVERFLOW:
        error = "STACK_OVERFLOW";
        break;
      case GL_STACK_UNDERFLOW:
        error = "STACK_UNDERFLOW";
        break;
      case GL_OUT_OF_MEMORY:
        error = "OUT_OF_MEMORY";
        break;
      case GL_INVALID_FRAMEBUFFER_OPERATION:
        error = "INVALID_FRAMEBUFFER_OPERATION";
        break;
    }
    errorLog("{} | {} ({})", error, file, line);
    return errorCode;
  }
  return errorCode;
}
#define glCheckError() glCheckError_(__FILE__, __LINE__)
#define ASSERT(x) \
  if (!(x)) __builtin_debugtrap();

#define GLCall(x) \
  x;              \
  ASSERT(glCheckError() == GL_NO_ERROR)

struct Output {
  unsigned int program;
  unsigned int VAO;
  unsigned int VBO;
  unsigned int EBO;
  int totalPoints;
  int totalIndicies;
};

void DebugEnv() {
  const char* glVersion = (const char*)glGetString(GL_VERSION);
  const char* glRenderer = (const char*)glGetString(GL_RENDERER);
  const char* glVendor = (const char*)glGetString(GL_VENDOR);

  log("OpenGL Version : {}", glVersion);
  log("OpenGL Model : {}", glRenderer);
  log("OpenGL Vendor : {}\n", glVendor);
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
    errorLog("{} Shader Compile Error : {}", typeStr, infoLog);
  } else {
    log("{} shader compile success", typeStr);
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
    errorLog("Program Link Error : {}", infoLog);
  } else {
    log("program link success");
  }

  glDeleteShader(vs);
  glDeleteShader(fs);

  return program;
}

Output CreateTriangle() {
  std::string vertexSrc = ReadFile("../assets/shaders/basic.vs");
  std::string fragmentSrc = ReadFile("../assets/shaders/basic.fs");

  unsigned int program = CreateShader(vertexSrc, fragmentSrc);

  // clang-format off
  std::vector<float> buffers = {
      -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f,// bl
      0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, // br
      0.5f, 0.5f, 0.0f,0.0f, 0.0f, 1.0f, // tr

      -0.5f, 0.5f, 0.0f,0.0f, 0.0f, 1.0f // tl
  };

  std::vector<unsigned int> indicies = {
    0,1,2,
    0,2,3
  };

  // clang-format on
  int count = 6;  // xyz, rgb
  int stride = count * sizeof(float);
  int totalPoints = buffers.size() / count;
  int totalIndicies = indicies.size();

  // 사이즈, 데이터의 시작 포인터를 GPU 에 할당.
  unsigned int VBO, VAO, EBO;
  glGenBuffers(1, &VBO);
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &EBO);

  glBindVertexArray(VAO);

  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, buffers.size() * sizeof(float), buffers.data(), GL_STATIC_DRAW);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, indicies.size() * sizeof(unsigned int), indicies.data(),
               GL_STATIC_DRAW);

  int posLayout = 0;
  // layout 0번 / 3개씩 써라 / float 타입 / normalize 안함 / 3 * float 간격 / 오프셋 0
  glVertexAttribPointer(posLayout, 3, GL_FLOAT, GL_FALSE, stride, (void*)0);
  glEnableVertexAttribArray(posLayout);

  int colorLayout = 1;
  glVertexAttribPointer(colorLayout, 3, GL_FLOAT, GL_FALSE, stride, (void*)(3 * sizeof(float)));
  glEnableVertexAttribArray(colorLayout);

  glBindVertexArray(0);

  return {program, VAO, VBO, EBO, totalPoints, totalIndicies};
}

int main() {
  GLFWwindow* window;

  if (!glfwInit()) {
    errorLog("Fail to Init GLFW");
    return -1;
  }

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  window = glfwCreateWindow(800, 600, "Hello opengl", NULL, NULL);
  if (!window) {
    glfwTerminate();
    errorLog("Fail to Create Window");
    return -1;
  }

  glfwMakeContextCurrent(window);
  glfwSwapInterval(1);

  DebugEnv();

  // create
  auto output = CreateTriangle();
  int colorLoc = glGetUniformLocation(output.program, "u_color");

  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
  while (!glfwWindowShouldClose(window)) {
    glClear(GL_COLOR_BUFFER_BIT);

    float time = glfwGetTime();

    // use
    glUseProgram(output.program);

    float r = (sin(time) + 1.0f) / 2.0f;
    float g = (sin(time + 2.0f) + 1.0f) / 2.0f;
    float b = (sin(time + 3.0f) + 1.0f) / 2.0f;
    glUniform4f(colorLoc, r, g, b, 1.0f);

    glBindVertexArray(output.VAO);  // begine(load)
    glDrawElements(GL_TRIANGLES, output.totalIndicies, GL_UNSIGNED_INT, NULL);
    glBindVertexArray(0);  // restore

    glfwSwapBuffers(window);

    glfwPollEvents();
  }

  // clean up
  glDeleteVertexArrays(1, &output.VAO);
  glDeleteBuffers(1, &output.VBO);
  glDeleteBuffers(1, &output.EBO);
  glDeleteProgram(output.program);

  glfwTerminate();

  return 0;
}
