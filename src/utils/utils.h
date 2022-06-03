#ifndef CG_LAB1_UTILS_H
#define CG_LAB1_UTILS_H

#include <GL/glew.h>

#include <iostream>
#include <string>
#include <vector>

#include <boost/format.hpp>

#define INVALID_OGL_VALUE 0xFFFFFFFF

#define GLExitIfError()                                               \
    {                                                                 \
        GLenum error = glGetError();                                  \
                                                                      \
        if (error != GL_NO_ERROR) {                                   \
            boost::format es("OpenGL error is %1%:%2%:%3$#x (%4%)");    \
            es % __FILE__ % __LINE__ % error % gluErrorString(error); \
            std::cerr << es << std::endl;                             \
            exit(0);                                                  \
        }                                                             \
    }

#define GLCheckError() (glGetError() == GL_NO_ERROR)

std::string readFile(const std::string &filePath);

template <class T>
inline void glBufferDataVector(GLenum target, const std::vector<T> &v, GLenum usage) {
    glBufferData(target, v.size() * sizeof(T), &v.front(), usage);
}

#endif  // CG_LAB1_UTILS_H
