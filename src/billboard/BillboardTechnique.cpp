#include "BillboardTechnique.h"

#include <utility>

#include <glm/gtc/type_ptr.hpp>

#include "../utils/utils.h"

BillboardTechnique::BillboardTechnique(
    std::string vertexShaderFilename,
    std::string geometryShaderFilename,
    std::string fragmentShaderFilename
)
    : mVertexShaderFilename(std::move(vertexShaderFilename)),
      mGeometryShaderFilename(std::move(geometryShaderFilename)),
      mFragmentShaderFilename(std::move(fragmentShaderFilename)) {}

bool BillboardTechnique::init() {
    if (!Technique::init()) return false;

    if (!addShaderFromFile(GL_VERTEX_SHADER, mVertexShaderFilename)) return false;

    if (!addShaderFromFile(GL_FRAGMENT_SHADER, mFragmentShaderFilename)) return false;

    if (!addShaderFromFile(GL_GEOMETRY_SHADER, mGeometryShaderFilename)) return false;

    if (!finalize()) return false;

    mVPLocation = getUniformLocation("gVP");
    mCameraPosLocation = getUniformLocation("gCameraPos");
    mColorMapLocation = getUniformLocation("gColorMap");
    mBillboardSizeLocation = getUniformLocation("gBillboardSize");

    if (mVPLocation == INVALID_UNIFORM_LOCATION ||
        mCameraPosLocation == INVALID_UNIFORM_LOCATION ||
        mColorMapLocation == INVALID_UNIFORM_LOCATION ||
        mBillboardSizeLocation == INVALID_UNIFORM_LOCATION)
        return false;

    return GLCheckError();
}

void BillboardTechnique::setVP(const glm::mat4& vp) const {
    glUniformMatrix4fv(mVPLocation, 1, GL_FALSE, glm::value_ptr(vp));
}

void BillboardTechnique::setCameraPosition(const glm::vec3& pos) const {
    glUniform3f(mCameraPosLocation, pos.x, pos.y, pos.z);
}

void BillboardTechnique::setColorTextureUnit(int textureUnit) const {
    glUniform1i(mColorMapLocation, textureUnit);
}

void BillboardTechnique::setBillboardSize(float billboardSize) const {
    glUniform1f(mBillboardSizeLocation, billboardSize);
}