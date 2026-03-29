#pragma once

#include "Debug.hpp"

class IndexBuffer {
 private:
  unsigned int id;
  int count = 0;

 public:
  IndexBuffer(const unsigned int* data, const int count) {
    GLCall(glGenBuffers(1, &id));
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id));
    GLCall(
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), data, GL_STATIC_DRAW));
    this->count = count;
  }
  ~IndexBuffer() { GLCall(glDeleteBuffers(1, &id)); }

  int GetCount() const { return count; }

  void Bind() const { GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id)); }
  void UnBind() const { GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0)); }
};
