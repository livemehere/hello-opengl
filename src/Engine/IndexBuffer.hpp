#pragma once

#include <OpenGL/gl.h>

#include "Debug.hpp"

class IndexBuffer {
 private:
  unsigned int id;

 public:
  IndexBuffer(const void* data, const int size) {
    GLCall(glGenBuffers(1, &id));
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id));
    GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, data, GL_STATIC_DRAW));
  }
  ~IndexBuffer() {}

  void Bind() const { GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id)); }

  void UnBind() const { GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0)); }
};
