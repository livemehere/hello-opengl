#pragma once

#include "Debug.hpp"

struct VertexElement {
  unsigned int type;
  unsigned int count;
  unsigned int normalized;
  static unsigned int GetSizeOfType(unsigned int type) {
    switch (type) {
      case GL_FLOAT:
        return 4;
    }

    ASSERT(false);
    return 0;
  }
};

class VertexBufferLayout {
 private:
  std::vector<VertexElement> elements;
  unsigned int stride = 0;

 public:
  VertexBufferLayout() {}
  ~VertexBufferLayout() = default;

  template <typename T>
  void Push(unsigned int count);
  unsigned int GetStride() const { return stride; };
  const std::vector<VertexElement>& GetElements() const { return elements; }
};

template <>
inline void VertexBufferLayout::Push<float>(unsigned int count) {
  elements.push_back({GL_FLOAT, count, GL_FALSE});
  stride += count * VertexElement::GetSizeOfType(GL_FLOAT);
}
