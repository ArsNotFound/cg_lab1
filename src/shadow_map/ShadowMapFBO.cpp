#include "ShadowMapFBO.h"

#include <iostream>

ShadowMapFBO::ShadowMapFBO()
    : mFBO(0),
      mShadowMap(0) {}

ShadowMapFBO::~ShadowMapFBO() {
    if (mFBO) glDeleteFramebuffers(1, &mFBO);

    if (mShadowMap) glDeleteFramebuffers(1, &mShadowMap);
}

bool ShadowMapFBO::init(unsigned int windowWidth, unsigned int windowHeight) {
    // Создаём FBO
    glGenFramebuffers(1, &mFBO);

    // Создаём буфер глубины
    glGenTextures(1, &mShadowMap);
    glBindTexture(GL_TEXTURE_2D, mShadowMap);
    glTexImage2D(
        GL_TEXTURE_2D,
        0,
        GL_DEPTH_COMPONENT,
        static_cast<GLint>(windowWidth),
        static_cast<GLint>(windowHeight),
        0,
        GL_DEPTH_COMPONENT,
        GL_FLOAT,
        nullptr
    );

    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, mFBO);
    glFramebufferTexture2D(
        GL_DRAW_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, mShadowMap, 0
    );

    glDrawBuffer(GL_NONE);

    GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);

    if (status != GL_FRAMEBUFFER_COMPLETE) {
        std::cerr << "FB error, status: " << status << std::endl;
        return false;
    }

    return true;
}

void ShadowMapFBO::bindForWriting() const {
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, mFBO);
}

void ShadowMapFBO::bindForReading(GLenum textureUnit) const {
    glActiveTexture(textureUnit);
    glBindTexture(GL_TEXTURE_2D, mShadowMap);
}
