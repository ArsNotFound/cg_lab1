#ifndef CG_LAB1_UTILS_H
#define CG_LAB1_UTILS_H

#include <GL/glew.h>

#include <iostream>
#include <string>
#include <vector>

#define INVALID_OGL_VALUE 0xFFFFFFFF

std::string readFile(const std::string &filePath);

template <class T>
inline void glBufferDataVector(GLenum target, const std::vector<T> &v, GLenum usage) {
    glBufferData(target, v.size() * sizeof(T), &v.front(), usage);
}

#endif  // CG_LAB1_UTILS_H
