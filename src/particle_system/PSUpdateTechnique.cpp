#include "PSUpdateTechnique.h"

#include <utility>
#include <vector>

PSUpdateTechnique::PSUpdateTechnique(
    std::string vertexShaderFilename, std::string geometryShaderFilename
)
    : mVertexShaderFilename(std::move(vertexShaderFilename)),
      mGeometryShaderFilename(std::move(geometryShaderFilename)) {}

bool PSUpdateTechnique::init() {
    if (!Technique::init()) return false;

    if (!addShaderFromFile(GL_VERTEX_SHADER, mVertexShaderFilename)) return false;

    if (!addShaderFromFile(GL_GEOMETRY_SHADER, mGeometryShaderFilename)) return false;

    std::vector<const GLchar*> varyings = {"Type1", "Position1", "Velocity1", "Age1"};

    glTransformFeedbackVaryings(
        mShaderProg,
        static_cast<int>(varyings.size()),
        &varyings.front(),
        GL_INTERLEAVED_ATTRIBS
    );

    if (!finalize()) return false;

    mDeltaTimeMillisLocation = getUniformLocation("gDeltaTimeMillis");
    mRandomTextureLocation = getUniformLocation("gRandomTexture");
    mTimeLocation = getUniformLocation("gTime");
    mLauncherLifetimeLocation = getUniformLocation("gLauncherLifetime");
    mShellLifetimeLocation = getUniformLocation("gShellLifetime");
    mSecondaryShellLifetimeLocation = getUniformLocation("gSecondaryShellLifetime");

    if (mDeltaTimeMillisLocation == INVALID_UNIFORM_LOCATION ||
        mTimeLocation == INVALID_UNIFORM_LOCATION ||
        mRandomTextureLocation == INVALID_UNIFORM_LOCATION ||
        mLauncherLifetimeLocation == INVALID_UNIFORM_LOCATION ||
        mShellLifetimeLocation == INVALID_UNIFORM_LOCATION ||
        mSecondaryShellLifetimeLocation == INVALID_UNIFORM_LOCATION) {
        return false;
    }

    return true;
}

void PSUpdateTechnique::setDeltaTimeMillis(float deltaTimeMillis) const
{
    glUniform1f(mDeltaTimeMillisLocation, deltaTimeMillis);
}


void PSUpdateTechnique::setTime(int time) const
{
    glUniform1f(mTimeLocation, static_cast<float>(time));
}


void PSUpdateTechnique::setRandomTextureUnit(int textureUnit) const
{
    glUniform1i(mRandomTextureLocation, textureUnit);
}


void PSUpdateTechnique::setLauncherLifetime(float lifetime) const
{
    glUniform1f(mLauncherLifetimeLocation, lifetime);
}


void PSUpdateTechnique::setShellLifetime(float lifetime) const
{
    glUniform1f(mShellLifetimeLocation, lifetime);
}


void PSUpdateTechnique::setSecondaryShellLifetime(float lifetime) const
{
    glUniform1f(mSecondaryShellLifetimeLocation, lifetime);
}