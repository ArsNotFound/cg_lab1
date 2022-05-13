#ifndef CG_LAB1_UTILS_H
#define CG_LAB1_UTILS_H

#include <GL/glew.h>

#include <string>
#include <vector>

std::string readFile(const std::string &filePath);

template <class T>
inline void glBufferDataVector(GLenum target, const std::vector<T> &v,
                               GLenum usage) {
    glBufferData(target, v.size() * sizeof(T), &v.front(), usage);
}

#endif  // CG_LAB1_UTILS_H
