#include "LightingTechnique.h"

#include <utility>

#include <boost/format.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "utils.h"

LightingTechnique::LightingTechnique(std::string vertexShaderFilename,
                                     std::string fragmentShaderFilename)
    : mVertexShaderFilename(std::move(vertexShaderFilename)),
      mFragmentShaderFilename(std::move(fragmentShaderFilename)) {
    mPointLightsLocation.resize(MAX_POINT_LIGHTS);
    mSpotLightsLocation.resize(MAX_SPOT_LIGHTS);
}

bool LightingTechnique::init() {
    if (!Technique::init()) return false;

    if (!addShaderFromFile(GL_VERTEX_SHADER, mVertexShaderFilename))
        return false;

    if (!addShaderFromFile(GL_FRAGMENT_SHADER, mFragmentShaderFilename))
        return false;

    if (!finalize()) return false;

    mWVPLocation = getUniformLocation("gWVP");
    mWorldMatrixLocation = getUniformLocation("gWorld");
    mSamplerLocation = getUniformLocation("gSampler");

    mEyeWorldPosLocation = getUniformLocation("gEyeWorldPos");
    mMatSpecularIntensityLocation = getUniformLocation("gMatSpecularIntensity");
    mMatSpecularPowerLocation = getUniformLocation("gMatSpecularPower");

    mDirLightLocation.color =
        getUniformLocation("gDirectionalLight.Base.Color");
    mDirLightLocation.ambientIntensity =
        getUniformLocation("gDirectionalLight.Base.AmbientIntensity");
    mDirLightLocation.diffuseIntensity =
        getUniformLocation("gDirectionalLight.Base.DiffuseIntensity");
    mDirLightLocation.direction =
        getUniformLocation("gDirectionalLight.Direction");

    mNumPointLightsLocation = getUniformLocation("gNumPointLights");

    for (auto i = 0; i < mPointLightsLocation.size(); i++) {
        auto name = boost::format("gPointLights[%1%].Base.Color") % i;
        mPointLightsLocation[i].color = getUniformLocation(name.str());

        name = boost::format("gPointLights[%1%].Base.AmbientIntensity") % i;
        mPointLightsLocation[i].ambientIntensity =
            getUniformLocation(name.str());

        name = boost::format("gPointLights[%1%].Base.DiffuseIntensity") % i;
        mPointLightsLocation[i].diffuseIntensity =
            getUniformLocation(name.str());

        name = boost::format("gPointLights[%1%].Position") % i;
        mPointLightsLocation[i].position = getUniformLocation(name.str());

        name = boost::format("gPointLights[%1%].Attenuation.Constant") % i;
        mPointLightsLocation[i].attenuation.constant =
            getUniformLocation(name.str());

        name = boost::format("gPointLights[%1%].Attenuation.Linear") % i;
        mPointLightsLocation[i].attenuation.linear =
            getUniformLocation(name.str());

        name = boost::format("gPointLights[%1%].Attenuation.Exp") % i;
        mPointLightsLocation[i].attenuation.exp =
            getUniformLocation(name.str());

        if (mPointLightsLocation[i].color == INVALID_UNIFORM_LOCATION ||
            mPointLightsLocation[i].ambientIntensity ==
                INVALID_UNIFORM_LOCATION ||
            mPointLightsLocation[i].diffuseIntensity ==
                INVALID_UNIFORM_LOCATION ||
            mPointLightsLocation[i].position == INVALID_UNIFORM_LOCATION ||
            mPointLightsLocation[i].attenuation.constant ==
                INVALID_UNIFORM_LOCATION ||
            mPointLightsLocation[i].attenuation.linear ==
                INVALID_UNIFORM_LOCATION ||
            mPointLightsLocation[i].attenuation.exp == INVALID_UNIFORM_LOCATION)
            return false;
    }

    mNumSpotLightsLocation = getUniformLocation("gNumSpotLights");

    for (auto i = 0; i < mSpotLightsLocation.size(); i++) {
        auto name = boost::format("gSpotLights[%1%].Base.Base.Color") % i;
        mSpotLightsLocation[i].color = getUniformLocation(name.str());

        name = boost::format("gSpotLights[%1%].Base.Base.AmbientIntensity") % i;
        mSpotLightsLocation[i].ambientIntensity =
            getUniformLocation(name.str());

        name = boost::format("gSpotLights[%1%].Base.Base.DiffuseIntensity") % i;
        mSpotLightsLocation[i].diffuseIntensity =
            getUniformLocation(name.str());

        name = boost::format("gSpotLights[%1%].Base.Position") % i;
        mSpotLightsLocation[i].position = getUniformLocation(name.str());

        name = boost::format("gSpotLights[%1%].Base.Attenuation.Constant") % i;
        mSpotLightsLocation[i].attenuation.constant =
            getUniformLocation(name.str());

        name = boost::format("gSpotLights[%1%].Base.Attenuation.Linear") % i;
        mSpotLightsLocation[i].attenuation.linear =
            getUniformLocation(name.str());

        name = boost::format("gSpotLights[%1%].Base.Attenuation.Exp") % i;
        mSpotLightsLocation[i].attenuation.exp = getUniformLocation(name.str());

        name = boost::format("gSpotLights[%1%].Direction") % i;
        mSpotLightsLocation[i].direction = getUniformLocation(name.str());

        name = boost::format("gSpotLights[%1%].Cutoff") % i;
        mSpotLightsLocation[i].cutoff = getUniformLocation(name.str());

        if (mSpotLightsLocation[i].color == INVALID_UNIFORM_LOCATION ||
            mSpotLightsLocation[i].ambientIntensity ==
                INVALID_UNIFORM_LOCATION ||
            mSpotLightsLocation[i].diffuseIntensity ==
                INVALID_UNIFORM_LOCATION ||
            mSpotLightsLocation[i].position == INVALID_UNIFORM_LOCATION ||
            mSpotLightsLocation[i].attenuation.constant ==
                INVALID_UNIFORM_LOCATION ||
            mSpotLightsLocation[i].attenuation.linear ==
                INVALID_UNIFORM_LOCATION ||
            mSpotLightsLocation[i].attenuation.exp ==
                INVALID_UNIFORM_LOCATION ||
            mSpotLightsLocation[i].direction == INVALID_UNIFORM_LOCATION ||
            mSpotLightsLocation[i].cutoff == INVALID_UNIFORM_LOCATION)
            return false;
    }

    if (mWVPLocation == INVALID_UNIFORM_LOCATION ||
        mSamplerLocation == INVALID_UNIFORM_LOCATION ||
        mEyeWorldPosLocation == INVALID_UNIFORM_LOCATION ||
        mMatSpecularIntensityLocation == INVALID_UNIFORM_LOCATION ||
        mMatSpecularPowerLocation == INVALID_UNIFORM_LOCATION ||
        mDirLightLocation.color == INVALID_UNIFORM_LOCATION ||
        mDirLightLocation.ambientIntensity == INVALID_UNIFORM_LOCATION ||
        mDirLightLocation.diffuseIntensity == INVALID_UNIFORM_LOCATION ||
        mDirLightLocation.direction == INVALID_UNIFORM_LOCATION ||
        mNumPointLightsLocation == INVALID_UNIFORM_LOCATION)
        return false;

    return true;
}

void LightingTechnique::setWVP(const glm::mat4 &wvp) const {
    glUniformMatrix4fv(mWVPLocation, 1, GL_FALSE, glm::value_ptr(wvp));
}

void LightingTechnique::setWorldMatrix(const glm::mat4 &worldInverse) const {
    glUniformMatrix4fv(mWorldMatrixLocation, 1, GL_FALSE,
                       glm::value_ptr(worldInverse));
}

void LightingTechnique::setTextureUnit(unsigned int textureUnit) const {
    glUniform1i(mSamplerLocation, static_cast<int>(textureUnit));
}

void LightingTechnique::setDirectionalLight(const DirectionLight &light) const {
    glUniform3f(mDirLightLocation.color, light.color.x, light.color.y,
                light.color.z);
    glUniform1f(mDirLightLocation.ambientIntensity, light.ambientIntensity);
    glm::vec3 direction = glm::normalize(light.direction);
    glUniform3f(mDirLightLocation.direction, direction.x, direction.y,
                direction.z);
    glUniform1f(mDirLightLocation.diffuseIntensity, light.diffuseIntensity);
}

void LightingTechnique::setEyeWorldPos(const glm::vec3 &eyeWorldPos) const {
    glUniform3f(mEyeWorldPosLocation, eyeWorldPos.x, eyeWorldPos.y,
                eyeWorldPos.z);
}

void LightingTechnique::setMatSpecularIntensity(float intensity) const {
    glUniform1f(mMatSpecularIntensityLocation, intensity);
}

void LightingTechnique::setMatSpecularPower(float power) const {
    glUniform1f(mMatSpecularPowerLocation, power);
}

void LightingTechnique::setPointLights(
    const std::vector<PointLight> &lights) const {
    assert(lights.size() <= MAX_POINT_LIGHTS);

    glUniform1i(mNumPointLightsLocation, static_cast<GLint>(lights.size()));

    for (auto i = 0; i < lights.size(); i++) {
        glUniform3f(mPointLightsLocation[i].color, lights[i].color.x,
                    lights[i].color.y, lights[i].color.z);
        glUniform1f(mPointLightsLocation[i].ambientIntensity,
                    lights[i].ambientIntensity);
        glUniform1f(mPointLightsLocation[i].diffuseIntensity,
                    lights[i].diffuseIntensity);
        glUniform3f(mPointLightsLocation[i].position, lights[i].position.x,
                    lights[i].position.y, lights[i].position.z);
        glUniform1f(mPointLightsLocation[i].attenuation.constant,
                    lights[i].attenuation.constant);
        glUniform1f(mPointLightsLocation[i].attenuation.linear,
                    lights[i].attenuation.linear);
        glUniform1f(mPointLightsLocation[i].attenuation.exp,
                    lights[i].attenuation.exp);
    }
}

void LightingTechnique::setSpotLights(
    const std::vector<SpotLight> &lights) const {
    assert(lights.size() < MAX_SPOT_LIGHTS);

    glUniform1i(mNumSpotLightsLocation, static_cast<GLint>(lights.size()));

    for (auto i = 0; i < lights.size(); i++) {
        glUniform3f(mSpotLightsLocation[i].color, lights[i].color.x,
                    lights[i].color.y, lights[i].color.z);
        glUniform1f(mSpotLightsLocation[i].ambientIntensity,
                    lights[i].ambientIntensity);
        glUniform1f(mSpotLightsLocation[i].diffuseIntensity,
                    lights[i].diffuseIntensity);
        glUniform3f(mSpotLightsLocation[i].position, lights[i].position.x,
                    lights[i].position.y, lights[i].position.z);
        glUniform1f(mSpotLightsLocation[i].attenuation.constant,
                    lights[i].attenuation.constant);
        glUniform1f(mSpotLightsLocation[i].attenuation.linear,
                    lights[i].attenuation.linear);
        glUniform1f(mSpotLightsLocation[i].attenuation.exp,
                    lights[i].attenuation.exp);
        auto direction = glm::normalize(lights[i].direction);
        glUniform3f(mSpotLightsLocation[i].direction, direction.x, direction.y,
                    direction.z);
        glUniform1f(mSpotLightsLocation[i].cutoff,
                    glm::radians(lights[i].cutoff));
    }
}
