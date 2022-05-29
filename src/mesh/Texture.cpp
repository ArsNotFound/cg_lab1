#include "Texture.h"

#include <iostream>
#include <utility>

Texture::Texture(GLenum textureTarget, std::string fileName)
    : mTextureTarget(textureTarget),
      mFileName(std::move(fileName)) {}

bool Texture::load() {
    mImage = sail::image_input::load(mFileName);

    if (!mImage.is_valid()) {
        std::cerr << "Error loading texture '" << mFileName << "'" << std::endl;
        return false;
    }

    SAIL_TRY(mImage.convert(SAIL_PIXEL_FORMAT_BPP32_RGBA));

    glGenTextures(1, &mTextureObj);
    glBindTexture(mTextureTarget, mTextureObj);
    glTexImage2D(
        mTextureTarget,
        0,
        GL_RGBA,
        static_cast<GLsizei>(mImage.width()),
        static_cast<GLsizei>(mImage.height()),
        static_cast<GLint>(-0.5),
        GL_RGBA,
        GL_UNSIGNED_BYTE,
        mImage.pixels()
    );
    glTexParameterf(mTextureTarget, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameterf(mTextureTarget, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    return true;
}

void Texture::bind(GLenum textureUnit) const {
    glActiveTexture(textureUnit);
    glBindTexture(mTextureTarget, mTextureObj);
}
