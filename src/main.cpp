#include <OpenGL/gl.h>
#include <OpenGL/glext.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <OpenGL/gl3.h>
#include <spdlog/common.h>
#include <spdlog/spdlog.h>

#include <vector>

#define log spdlog::info
#define errorLog spdlog::error

struct Output {
  unsigned int program;
  unsigned int VAO;
  unsigned int VBO;
  int totalPoints;
};

static void DebugEnv() {
  const char* glVersion = (const char*)glGetString(GL_VERSION);
  const char* glRenderer = (const char*)glGetString(GL_RENDERER);
  const char* glVendor = (const char*)glGetString(GL_VENDOR);

  log("OpenGL Version : {}", glVersion);
  log("OpenGL Model : {}", glRenderer);
  log("OpenGL Vendor : {}\n", glVendor);
}

static unsigned int CompileShader(GLenum type, const std::string& shaderSource) {
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

static unsigned int CreateShader(const std::string& vertexShader,
                                 const std::string& fragmentShader) {
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
  }

  glDeleteShader(vs);
  glDeleteShader(fs);

  log("program link success");
  return program;
}

Output CreateTriangle() {
  std::string vertexSrc = R"(
    #version 330 core

    layout (location = 0) in vec3 pos;
    layout (location = 1) in vec3 inColor;

    out vec3 color;

    void main() {
      gl_Position = vec4(pos.xyz,1.0);
      color = inColor;
    }
  )";
  std::string fragmentSrc = R"(
    #version 330 core

    in vec3 color;
    out vec4 fragment;

    void main() {
      fragment = vec4(color, 1.0f);
    }
  )";

  unsigned int program = CreateShader(vertexSrc, fragmentSrc);

  // clang-format off
  std::vector<float> buffers = {
      -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f,// left
      0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, // right
      0.0f, 0.5f, 0.0f,0.0f, 0.0f, 1.0f // top
  };
  // clang-format on
  int count = 6;
  int stride = count * sizeof(float);
  int totalPoints = buffers.size() / count;

  // 사이즈, 데이터의 시작 포인터를 GPU 에 할당.
  unsigned int VBO, VAO;
  glGenBuffers(1, &VBO);
  glGenVertexArrays(1, &VAO);

  glBindVertexArray(VAO);

  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, buffers.size() * sizeof(float), buffers.data(), GL_STATIC_DRAW);

  int posLayout = 0;
  // layout 0번 / 3개씩 써라 / float 타입 / normalize 안함 / 3 * float 간격 / 오프셋 0
  glVertexAttribPointer(posLayout, 3, GL_FLOAT, GL_FALSE, stride, (void*)0);
  glEnableVertexAttribArray(posLayout);

  int colorLayout = 1;
  glVertexAttribPointer(colorLayout, 3, GL_FLOAT, GL_FALSE, stride, (void*)(3 * sizeof(float)));
  glEnableVertexAttribArray(colorLayout);

  glBindVertexArray(0);

  return {program, VAO, VBO, totalPoints};
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

  DebugEnv();

  // create
  auto output = CreateTriangle();

  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
  while (!glfwWindowShouldClose(window)) {
    glClear(GL_COLOR_BUFFER_BIT);

    // use
    glUseProgram(output.program);
    glBindVertexArray(output.VAO);
    glDrawArrays(GL_TRIANGLES, 0, output.totalPoints);

    glfwSwapBuffers(window);

    glfwPollEvents();
  }

  // clean up
  glDeleteVertexArrays(1, &output.VAO);
  glDeleteBuffers(1, &output.VBO);
  glDeleteProgram(output.program);

  glfwTerminate();

  return 0;
}
