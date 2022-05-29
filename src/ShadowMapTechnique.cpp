#include "ShadowMapTechnique.h"

#include <utility>

#include <glm/gtc/type_ptr.hpp>

ShadowMapTechnique::ShadowMapTechnique(std::string vertexShaderFilename,
                                       std::string fragmentShaderFilename)
    : mVertexShaderFilename(std::move(vertexShaderFilename)),
      mFragmentShaderFilename(std::move(fragmentShaderFilename)) {}

bool ShadowMapTechnique::init() {
    if (!Technique::init()) return false;

    if (!addShaderFromFile(GL_VERTEX_SHADER, mVertexShaderFilename))
        return false;

    if (!addShaderFromFile(GL_FRAGMENT_SHADER, mFragmentShaderFilename))
        return false;

    if (!finalize()) return false;

    mWVPLocation = getUniformLocation("gWVP");
    mTextureLocation = getUniformLocation("gShadowMap");

    if (mWVPLocation == INVALID_UNIFORM_LOCATION ||
        mTextureLocation == INVALID_UNIFORM_LOCATION)
        return false;
    return true;
}

void ShadowMapTechnique::setWVP(const glm::mat4 &wvp) const {
    glUniformMatrix4fv(mWVPLocation, 1, GL_FALSE, glm::value_ptr(wvp));
}

void ShadowMapTechnique::setTextureUnit(unsigned int textureUnit) const {
    glUniform1i(mTextureLocation, static_cast<GLint>(textureUnit));
}
