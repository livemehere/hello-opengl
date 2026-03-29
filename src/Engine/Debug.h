#pragma once

#include <OpenGL/gl.h>
#include <OpenGL/gl3.h>
#include <spdlog/common.h>
#include <spdlog/spdlog.h>

#define LOG spdlog::info
#define ERROR_LOG spdlog::error

#define ASSERT(x) \
  if (!(x)) __builtin_debugtrap();
#define GLCall(x)                                \
  do {                                           \
    GLClearError();                              \
    x;                                           \
    ASSERT(GLLogCall(#x, __FILE__, __LINE____)); \
  } while (0)

inline void GLClearError() { while (glGetError() != GL_NO_ERROR); }

inline bool GLLogCall(const char* func, const char* file, int line) {
  while (GLenum error = glGetError()) {
    spdlog::error("[OpenGL Error] ({:#04x}): {} {} : {}", error, func, file, line);
    return false;
  }
  return true;
}
