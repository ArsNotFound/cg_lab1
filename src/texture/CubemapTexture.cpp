#include "CubemapTexture.h"

#include <iostream>

const std::vector<GLenum> types = { // NOLINT(cert-err58-cpp)
    GL_TEXTURE_CUBE_MAP_POSITIVE_X,
    GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
    GL_TEXTURE_CUBE_MAP_POSITIVE_Y,
    GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
    GL_TEXTURE_CUBE_MAP_POSITIVE_Z,
    GL_TEXTURE_CUBE_MAP_NEGATIVE_Z
};

CubemapTexture::CubemapTexture(
    const std::string &directory,
    const std::string &poxXFilename,
    const std::string &negXFilename,
    const std::string &posYFilename,
    const std::string &negYFilename,
    const std::string &posZFilename,
    const std::string &negZFilename
) {
    mFilenames.resize(6);

    auto it = directory.end();
    it--;
    auto baseDir = (*it == '/') ? directory : directory + "/";

    mFilenames[0] = baseDir + poxXFilename;
    mFilenames[1] = baseDir + negXFilename;
    mFilenames[2] = baseDir + posYFilename;
    mFilenames[3] = baseDir + negYFilename;
    mFilenames[4] = baseDir + posZFilename;
    mFilenames[5] = baseDir + negZFilename;

    mTextureObj = 0;
}

CubemapTexture::~CubemapTexture() {
    if (mTextureObj != 0)
        glDeleteTextures(1, &mTextureObj);
}

bool CubemapTexture::load() {
    glGenTextures(1, &mTextureObj);
    glBindTexture(GL_TEXTURE_CUBE_MAP, mTextureObj);

    sail::image image;

    for (auto i = 0; i < types.size(); i++) {
        image = sail::image_input::load(mFilenames[i]);
        if (!image.is_valid()) {
            std::cerr << "Error loading textures '" << mFilenames[i] << "'" << std::endl;
            return false;
        }

        SAIL_TRY(image.convert(SAIL_PIXEL_FORMAT_BPP24_RGB));

        glTexImage2D(
            types[i],
            0,
            GL_RGB,
            static_cast<GLsizei>(image.width()),
            static_cast<GLsizei>(image.height()),
            0,
            GL_RGB,
            GL_UNSIGNED_BYTE,
            image.pixels()
        );
    }

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    return true;
}

void CubemapTexture::bind(GLenum textureUnit) const {
    glActiveTexture(textureUnit);
    glBindTexture(GL_TEXTURE_CUBE_MAP, mTextureObj);
}