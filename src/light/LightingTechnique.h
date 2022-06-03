#ifndef CG_LAB1_LIGHTINGTECHNIQUE_H
#define CG_LAB1_LIGHTINGTECHNIQUE_H

#include <vector>

#include <glm/glm.hpp>

#include "../technique/Technique.h"

#define MAX_POINT_LIGHTS 3
#define MAX_SPOT_LIGHTS 3

struct BaseLight {
        glm::vec3 color;
        float ambientIntensity;
        float diffuseIntensity;

        BaseLight()
            : color(glm::vec3(0.0f)),
              ambientIntensity(0.0f),
              diffuseIntensity(0.0f) {}
};

struct DirectionLight : public BaseLight {
        glm::vec3 direction;
};

struct PointLight : public BaseLight {
        glm::vec3 position;

        struct {
                float constant;
                float linear;
                float exp;
        } attenuation;

        PointLight()
            : BaseLight(),
              position(glm::vec3(0.0f)) {
            attenuation.constant = 1.0f;
            attenuation.linear = 0.0f;
            attenuation.exp = 0.0f;
        }
};

struct SpotLight : public PointLight {
        glm::vec3 direction;
        float cutoff;

        SpotLight()
            : direction(glm::vec3(0.0f)),
              cutoff(0.0f) {}
};

class LightingTechnique : public Technique {
    public:
        LightingTechnique(
            std::string vertexShaderFilename, std::string fragmentShaderFilename
        );

        bool init() override;

        void setWVP(const glm::mat4 &wvp) const;

        void setWorldMatrix(const glm::mat4 &worldInverse) const;

        void setColorTextureUnit(int textureUnit) const;

        void setShadowMapTextureUnit(int textureUnit) const;

        void setNormalMapTextureUnit(int textureUnit) const;

        void setDirectionalLight(const DirectionLight &light) const;

        void setEyeWorldPos(const glm::vec3 &eyeWorldPos) const;

        void setMatSpecularIntensity(float intensity) const;

        void setMatSpecularPower(float power) const;

        void setPointLights(const std::vector<PointLight> &lights) const;

        void setSpotLights(const std::vector<SpotLight> &lights) const;

        void setLightWVP(const glm::mat4 &lightWVP) const;

    private:
        std::string mVertexShaderFilename;
        std::string mFragmentShaderFilename;

        GLint mWVPLocation;
        GLint mWorldMatrixLocation;

        GLint mEyeWorldPosLocation;
        GLint mMatSpecularIntensityLocation;
        GLint mMatSpecularPowerLocation;

        GLint mLightWVPLocation;

        GLint mColorMapLocation;
        GLint mShadowMapLocation;
        GLint mNormalMapLocation;

        struct sBaseLight {
                GLint color;
                GLint ambientIntensity;
                GLint diffuseIntensity;
        };

        struct : public sBaseLight {
                GLint direction;
        } mDirLightLocation;

        struct sPointLight {
                GLint color;
                GLint ambientIntensity;
                GLint diffuseIntensity;
                GLint position;

                struct {
                        GLint constant;
                        GLint linear;
                        GLint exp;
                } attenuation;
        };

        GLint mNumPointLightsLocation;
        std::vector<sPointLight> mPointLightsLocation;

        struct sSpotLight : public sPointLight {
                GLint direction;
                GLint cutoff;
        };

        GLint mNumSpotLightsLocation;
        std::vector<sSpotLight> mSpotLightsLocation;
};

#endif  // CG_LAB1_LIGHTINGTECHNIQUE_H
