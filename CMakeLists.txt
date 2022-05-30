cmake_minimum_required(VERSION 3.21)
project(cg_lab1)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_VERBOSE_MAKEFILE ON)
set(GCC_COVERAGE_COMPILE_FLAGS "-Wall")

find_package(glm REQUIRED)
find_package(SailC++ REQUIRED)
find_package(Boost)
find_package(assimp)

file(GLOB_RECURSE SOURCES src/*.cpp src/*.h)

add_executable(cg_lab1 ${SOURCES})

target_link_libraries(
        cg_lab1
        GL
        GLU
        glut
        GLEW
        glm
        assimp
        SAIL::sail-c++
        ${ImageMagick_LIBRARIES}
        ${ImageMagick_Magick++_LIBRARIES}
        ${Boost_LIBRARIES}
        ${SailC++_LIBRARIES}
        )

include_directories(
        cg_lab1
        ${ImageMagick_INCLUDE_DIRS}
        ${ImageMagick_Magick++_INCLUDE_DIRS}
        ${Boost_INCLUDE_DIRS}
        ${SailC++_INCLUDE_DIRS}
)

get_property(dirs DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY INCLUDE_DIRECTORIES)
foreach(dir ${dirs})
    message(STATUS "dir='${dir}'")
endforeach()

add_compile_options(-Wall)

if(UNIX)
    set_target_properties(cg_lab1 PROPERTIES SUFFIX ".bin")
endif()