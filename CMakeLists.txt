cmake_minimum_required(VERSION 4.2)
project(hello-opengl CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS "src/*.cpp", "src/*.hpp")

add_executable(${PROJECT_NAME} ${SOURCES})

