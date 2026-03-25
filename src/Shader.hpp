#pragma once

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
  int count;

  std::vector<GLfloat> vertices;

  Shader(const std::string &vs, const std::string &fs,
         const std::vector<GLfloat> &vertices) {

    count = vertices.size() / 3;

    // shader compile
    const char *vsSrc = vs.c_str();
    const char *fsSrc = fs.c_str();

    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vsSrc, NULL);
    glCompileShader(vertexShader);

    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fsSrc, NULL);
    glCompileShader(fragmentShader);

    program = glCreateProgram();
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);
    glLinkProgram(program);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // alloc 왜 여긴 &VAO, &VBO 일까? = C의 쓰기모드
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    // recording start 여긴 왜 & 없이? = C 의 읽기 모드
    glBindVertexArray(VAO);

    // data 할당
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat),
                 vertices.data(), GL_STATIC_DRAW);

    // VBO 데이터 해석 방법 정의
    // vertex Shader 의 layout 0 번 포트 사용 / 3개씩 읽어라 / float 타입이다 /
    // 0~1 정규화 여부 / 읽을 단위? 이걸 왜또주지 size 3을 줬는데도.. / 빈포인터
    // 하나 전달?
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat),
                          (void *)0);

    // layout port 활성화 (이거랑 위에 함수 첫번째 인자랑 동일해야하나?)
    glEnableVertexAttribArray(0);

    // 하나의 컨텍스트(붓) 이기 때문에, 내가 할일이 끝났으면, 내 리소스는
    // 사용하지 않도록 정리 작업 (선택적)
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
  }

  void Use() {
    glUseProgram(program);
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, count);
  }
};
