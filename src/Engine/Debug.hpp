#pragma once

#include <OpenGL/gl3.h>
#include <spdlog/common.h>
#include <spdlog/spdlog.h>

#define LOG spdlog::info
#define ERROR_LOG spdlog::error

#define ASSERT(x) \
  if (!(x)) __builtin_debugtrap();
#define GLCall(x) \
  GLClearError(); \
  x;              \
  ASSERT(GLLogCall(#x, __FILE__, __LINE__));

inline void GLClearError() { while (glGetError() != GL_NO_ERROR); }

inline bool GLLogCall(const char* func, const char* file, int line) {
  while (GLenum error = glGetError()) {
    std::string errStr;

    switch (error) {
      case GL_INVALID_ENUM:
        errStr = "INVALID_ENUM";
        break;
      case GL_INVALID_VALUE:
        errStr = "INVALID_VALUE";
        break;
      case GL_INVALID_OPERATION:
        errStr = "INVALID_OPERATION";
        break;
      case GL_OUT_OF_MEMORY:
        errStr = "OUT_OF_MEMORY";
        break;
      case GL_INVALID_FRAMEBUFFER_OPERATION:
        errStr = "INVALID_FRAMEBUFFER_OPERATION";
        break;
        "Unknown";
    };
    spdlog::error("[OpenGL Error] ({}): {} {} : {}", errStr, func, file, line);
    return false;
  }
  return true;
}
