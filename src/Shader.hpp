#pragma once

#include <iostream>
#include <OpenGL/gl.h>
#include <OpenGL/glext.h>
#define GLFW_INCLUDE_GLCOREARB // glGenVertexArrays << 이거쓰려면 gl3.h
                               // 가져와야됨.
#include <GLFW/glfw3.h>
#include <OpenGL/gltypes.h>
#include <string>
#include <vector>

struct Shader {
  GLuint program;
  GLuint VAO;
  GLuint VBO;
  GLuint EBO;
  int count;
  int indiciesCount;

  std::vector<GLfloat> vertices;

  Shader(const std::string &vs, const std::string &fs,
         const std::vector<GLfloat> &vertices,
         const std::vector<GLuint> &indices) {

    count = vertices.size() / 5; // x,y,z,u,v
    indiciesCount = indices.size();

    // shader compile
    const char *vsSrc = vs.c_str();
    const char *fsSrc = fs.c_str();

    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vsSrc, NULL);
    glCompileShader(vertexShader);

    // vertex shader 컴파일 에러 체크
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success) {
      glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
      std::cout << "ERROR: Vertex shader compilation failed\n" << infoLog << std::endl;
    }

    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fsSrc, NULL);
    glCompileShader(fragmentShader);

    // fragment shader 컴파일 에러 체크
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success) {
      glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
      std::cout << "ERROR: Fragment shader compilation failed\n" << infoLog << std::endl;
    }

    program = glCreateProgram();
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);
    glLinkProgram(program);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // alloc 왜 여긴 &VAO, &VBO 일까? = C의 쓰기모드
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    // recording start 여긴 왜 & 없이? = C 의 읽기 모드
    glBindVertexArray(VAO);

    // data 할당
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat),
                 vertices.data(), GL_STATIC_DRAW);

    // EBO
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint),
                 indices.data(), GL_STATIC_DRAW);

    // VBO 데이터 해석 방법 정의
    // 위치 attribute (location 0) - x, y, z
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat),
                          (void *)0);
    glEnableVertexAttribArray(0);

    // 텍스처 좌표 attribute (location 1) - u, v
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat),
                          (void *)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

    // 하나의 컨텍스트(붓) 이기 때문에, 내가 할일이 끝났으면, 내 리소스는
    // 사용하지 않도록 정리 작업 (선택적)
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
  }

  void Use() {
    glUseProgram(program);
    glBindVertexArray(VAO);
    // glDrawArrays(GL_TRIANGLES, 0, count);
    glDrawElements(GL_TRIANGLES, indiciesCount, GL_UNSIGNED_INT, 0);
  }
};
