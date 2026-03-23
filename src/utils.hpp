#pragma once

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

std::string readFile(const char *filePath) {
  std::string shaderCode;
  std::ifstream shaderFile;

  shaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
  try {
    shaderFile.open(filePath);
    std::stringstream shaderStream;

    shaderStream << shaderFile.rdbuf();
    shaderFile.close();
    shaderCode = shaderStream.str();
  } catch (std::ifstream::failure &e) {
    std::cout << "Error on open file : " << filePath << std::endl;
  }

  return shaderCode;
}
