cmake_minimum_required(VERSION 3.21)
project(cg_lab1)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_VERBOSE_MAKEFILE ON)

find_package(glm REQUIRED)
find_package(ImageMagick)
find_package(ImageMagick COMPONENTS Magick++)
find_package(Boost)
find_package(assimp)

file(GLOB SOURCES src/*.cpp src/*.h)

add_executable(cg_lab1 ${SOURCES})

add_definitions( -DMAGICKCORE_QUANTUM_DEPTH=16 )
add_definitions( -DMAGICKCORE_HDRI_ENABLE=0 )

target_link_libraries(
        cg_lab1
        GL
        GLU
        glut
        GLEW
        glm
        assimp
        ${ImageMagick_LIBRARIES}
        ${ImageMagick_Magick++_LIBRARIES}
        ${Boost_LIBRARIES}
        )

include_directories(
        cg_lab1
        ${ImageMagick_INCLUDE_DIRS}
        ${ImageMagick_Magick++_INCLUDE_DIRS}
        ${Boost_INCLUDE_DIRS}
)

add_compile_options(-Wall)

if(UNIX)
    set_target_properties(cg_lab1 PROPERTIES SUFFIX ".bin")
endif()