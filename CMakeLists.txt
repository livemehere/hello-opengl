cmake_minimum_required(VERSION 3.20)
project(hello-opengl LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED on)

file(GLOB_RECURSE sources CONFIGURE_DEPENDS "src/*.cpp" "src/*.hpp")

add_executable(${PROJECT_NAME} ${sources})

target_include_directories(${PROJECT_NAME} PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}/external/include)

target_link_libraries(${PROJECT_NAME} PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}/external/lib/libglfw.3.dylib
  "-framework OpenGL"
)

