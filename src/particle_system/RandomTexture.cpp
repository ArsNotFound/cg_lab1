#include "RandomTexture.h"

#include <vector>

#include <glm/glm.hpp>

#include "../utils/utils.h"

RandomTexture::RandomTexture()
    : mTextureObj(0) {}

RandomTexture::~RandomTexture() {
    if (mTextureObj != 0) glDeleteTextures(1, &mTextureObj);
}

bool RandomTexture::init(unsigned int size) {
    std::vector<glm::vec3> randomData(size);
    for (auto i = 0; i < size; i++) {
        randomData[i].x = randomFloat();
        randomData[i].y = randomFloat();
        randomData[i].z = randomFloat();
    }

    glGenTextures(1, &mTextureObj);
    glBindTexture(GL_TEXTURE_1D, mTextureObj);
    glTexImage1D(
        GL_TEXTURE_1D,
        0,
        GL_RGB,
        static_cast<int>(size),
        0.0f,
        GL_RGB,
        GL_FLOAT,
        &randomData.front()
    );

    glTexParameterf(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_1D, GL_TEXTURE_WRAP_S, GL_REPEAT);

    return GLCheckError();
}

void RandomTexture::bind(GLenum textureUnit) const {
    glActiveTexture(textureUnit);
    glBindTexture(GL_TEXTURE_1D, mTextureObj);
}