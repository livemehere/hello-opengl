#pragma once

#include <format>
#include <fstream>
#include <sstream>
#include <string>

inline std::string ReadFile(std::string path) {
  std::ifstream file(path);
  if (!file.is_open()) {
    throw std::runtime_error(std::format("Can not Open the File : {} ", path));
  }

  std::stringstream buffer;
  buffer << file.rdbuf();
  return buffer.str();
}
