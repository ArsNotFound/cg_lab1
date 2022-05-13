//
// Created by arslan on 06.05.22.
//

#include <iostream>
#include <utility>
#include "Texture.h"

Texture::Texture(GLenum textureTarget, std::string fileName) :
        mTextureTarget(textureTarget), mFileName(std::move(fileName)) {}

bool Texture::load() {
    try {
        mPImage = new Magick::Image(mFileName);
        mPImage->write(&mBlob, "RGBA");
    } catch (Magick::Error &error) {
        std::cerr << "Error loading texture '" << mFileName << "': " << error.what() << std::endl;
        return false;
    }

    glGenTextures(1, &mTextureObj);
    glBindTexture(mTextureTarget, mTextureObj);
    glTexImage2D(mTextureTarget,
                 0,
                 GL_RGB,
                 static_cast<GLsizei>(mPImage->columns()),
                 static_cast<GLsizei>(mPImage->rows()),
                 static_cast<GLint>(-0.5),
                 GL_RGBA,
                 GL_UNSIGNED_BYTE,
                 mBlob.data());
    glTexParameterf(mTextureTarget, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameterf(mTextureTarget, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    return true;
}

void Texture::bind(GLenum textureUnit) const {
    glActiveTexture(textureUnit);
    glBindTexture(mTextureTarget, mTextureObj);
}
