#pragma once

#include <GLFW/glfw3.h>
#include <OpenGL/gltypes.h>
#include <string>
#include <vector>

struct Shader {
  GLuint VAO;
  GLuint VBO;

  std::string vs;
  std::string fs;

  std::vector<GLfloat> vertices;

  Shader(const std::string &vs, const std::string &fs,
         const std::vector<GLfloat> &vertices)
      : vs(vs), fs(fs), vertices(vertices) {}
};
