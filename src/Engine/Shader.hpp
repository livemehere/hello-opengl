#pragma once

#include <OpenGL/gl.h>

#include <unordered_map>

#include "Debug.hpp"
#include "Utils.hpp"

struct ShaderSources {
  std::string vertexSource;
  std::string fragmentSource;
};

class Shader {
 private:
  unsigned int program;
  std::unordered_map<std::string, int> uniformCache;

  ShaderSources ParseShader(const std::string& vertexShaderFilePath,
                            const std::string& fragmentShaderFilePath) {
    std::string vertexSource = ReadFile(vertexShaderFilePath);
    std::string fragmentSource = ReadFile(fragmentShaderFilePath);
    return {vertexSource, fragmentSource};
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

  int GetUniformLocation(const std::string& name) {
    if (uniformCache.find(name) != uniformCache.end()) {
      return uniformCache[name];
    }

    GLCall(int location = glGetUniformLocation(program, name.c_str()));
    if (location == -1) {
      ERROR_LOG("Can not find uniform : {}", name);
    }

    uniformCache[name] = location;
    return location;
  }

 public:
  Shader(const std::string& vertexShaderFilePath, const std::string& fragmentShaderFilePath) {
    auto sources = ParseShader(vertexShaderFilePath, fragmentShaderFilePath);
    program = CreateShader(sources.vertexSource, sources.fragmentSource);
  }
  ~Shader() { GLCall(glDeleteProgram(program)); }

  void SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3) {
    int loc = GetUniformLocation(name);
    GLCall(glUniform4f(loc, v0, v1, v2, v3));
  }

  void Bind() const { GLCall(glUseProgram(program)); }
  void UnBind() { GLCall(glUseProgram(0)); }
};
