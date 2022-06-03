#ifndef CG_LAB1_SHADOWMAPTECHNIQUE_H
#define CG_LAB1_SHADOWMAPTECHNIQUE_H

#include <glm/glm.hpp>

#include "../technique/Technique.h"

class ShadowMapTechnique : public Technique {
    public:
        ShadowMapTechnique(
            std::string vertexShaderFilename, std::string fragmentShaderFilename
        );

        bool init() override;

        void setWVP(const glm::mat4& wvp) const;

        void setTextureUnit(unsigned int textureUnit) const;

    private:
        std::string mVertexShaderFilename;
        std::string mFragmentShaderFilename;

        GLint mWVPLocation;
        GLint mTextureLocation;
};

#endif  // CG_LAB1_SHADOWMAPTECHNIQUE_H
