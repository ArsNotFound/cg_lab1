#include "SkyboxTechnique.h"

#include <utility>

#include <glm/gtc/type_ptr.hpp>

#include "../utils/utils.h"

SkyboxTechnique::SkyboxTechnique(
    std::string vertexShaderFilename, std::string fragmentShaderFilename
)
    : mVertexShaderFilename(std::move(vertexShaderFilename)),
      mFragmentShaderFilename(std::move(fragmentShaderFilename)) {}

SkyboxTechnique::~SkyboxTechnique() = default;

bool SkyboxTechnique::init() {
    if (!Technique::init()) return false;

    if (!addShaderFromFile(GL_VERTEX_SHADER, mVertexShaderFilename)) return false;

    if (!addShaderFromFile(GL_FRAGMENT_SHADER, mFragmentShaderFilename)) return false;

    if (!finalize()) return false;

    mWVPLocation = getUniformLocation("gWVP");
    mTextureLocation = getUniformLocation("gCubemapTexture");

    if (mWVPLocation == INVALID_UNIFORM_LOCATION ||
        mTextureLocation == INVALID_UNIFORM_LOCATION)
        return false;

    return GLCheckError();
}

void SkyboxTechnique::setWVP(const glm::mat4& wvp) const {
    glUniformMatrix4fv(mWVPLocation, 1, GL_FALSE, glm::value_ptr(wvp));
}

void SkyboxTechnique::setTextureUnit(GLint textureUnit) const {
    glUniform1i(mTextureLocation, textureUnit);
}
