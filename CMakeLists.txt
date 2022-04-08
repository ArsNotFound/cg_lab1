cmake_minimum_required(VERSION 3.21)
project(cg_lab1)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_VERBOSE_MAKEFILE ON)

find_package(glm REQUIRED)

add_executable(cg_lab1 main.cpp pipeline.cpp pipeline.h)

target_link_libraries(cg_lab1 GL GLU glut GLEW glm)

if(UNIX)
    set_target_properties(cg_lab1 PROPERTIES SUFFIX ".bin")
endif()