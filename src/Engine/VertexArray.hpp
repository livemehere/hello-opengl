#pragma once

#include "Debug.hpp"
#include "VertexBuffer.hpp"
#include "VertexBufferyLayout.hpp"

class VertexArray {
 private:
  unsigned int id;

 public:
  VertexArray() { GLCall(glGenVertexArrays(1, &id)); }
  ~VertexArray() {
    GLCall(glDeleteVertexArrays(1, &id));
    LOG("VertexArray 소멸됨");
  }

  void AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout) {
    Bind();
    vb.Bind();

    const auto& elements = layout.GetElements();
    auto stride = layout.GetStride();
    unsigned int offset = 0;
    for (unsigned int i = 0; i < elements.size(); i++) {
      const auto& element = elements[i];

      GLCall(glEnableVertexAttribArray(i));
      GLCall(glVertexAttribPointer(i, element.count, element.type, element.normalized, stride,
                                   (const void*)(uintptr_t)offset));
      offset += element.count * VertexElement::GetSizeOfType(element.type);
    }
  }

  void Bind() const { GLCall(glBindVertexArray(id)); }
  void UnBind() const { GLCall(glBindVertexArray(0)); }
};
