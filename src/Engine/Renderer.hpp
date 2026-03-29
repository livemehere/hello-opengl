#pragma once

#include "Debug.hpp"
#include "IndexBuffer.hpp"
#include "Shader.hpp"
#include "VertexArray.hpp"

class Renderer {
 private:
 public:
  Renderer() {}
  ~Renderer() {}

  void Clear() const { glClear(GL_COLOR_BUFFER_BIT); }
  void Draw(const VertexArray& va, const IndexBuffer& ib) const {
    va.Bind();
    ib.Bind();
    GLCall(glDrawElements(GL_TRIANGLES, ib.GetCount(), GL_UNSIGNED_INT, NULL));

    // va.UnBind();
    // vb.UnBind();
    // ib.UnBind();
  }
};
