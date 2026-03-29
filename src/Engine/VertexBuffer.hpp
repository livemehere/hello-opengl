#pragma once

#include "Debug.hpp"

class VertexBuffer {
 private:
  unsigned int id;

 public:
  VertexBuffer(const void* data, unsigned int size) {
    GLCall(glGenBuffers(1, &id));
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, id));
    GLCall(glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW));
  }
  ~VertexBuffer() { GLCall(glDeleteBuffers(1, &id)); }

  void Bind() const { GLCall(glBindBuffer(GL_ARRAY_BUFFER, id)); }
  void UnBind() const { GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0)); }
};
