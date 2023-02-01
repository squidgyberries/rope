cmake_minimum_required(VERSION 3.20)
project(rope LANGUAGES C CXX)

set(CMAKE_C_STANDARD 17)
set(CMAKE_CXX_STANDARD 17)
add_compile_options(-Wall -Wextra -Wpedantic -Wno-unused-parameter)

if(UNIX AND NOT APPLE)
  set(OpenGL_GL_PREFERENCE GLVND)
endif()
add_subdirectory(raylib)

add_subdirectory(fmt)

set(
  SRC
  src/rope.cpp
)

add_executable(${PROJECT_NAME} ${SRC})
target_link_libraries(${PROJECT_NAME} raylib fmt)
