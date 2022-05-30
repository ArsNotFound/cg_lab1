#ifndef CG_LAB1_SKYBOXTECHNIQUE_H
#define CG_LAB1_SKYBOXTECHNIQUE_H

#include <string>

#include <glm/glm.hpp>

#include "Technique.h"

class SkyboxTechnique : public Technique {
    public:
        SkyboxTechnique(
            std::string vertexShaderFilename, std::string fragmentShaderFilename
        );

        ~SkyboxTechnique();

        bool init() override;

        void setWVP(const glm::mat4& wvp) const;

        void setTextureUnit(GLint textureUnit) const;

    private:
        std::string mVertexShaderFilename;
        std::string mFragmentShaderFilename;

        GLint mWVPLocation;
        GLint mTextureLocation;
};

#endif  // CG_LAB1_SKYBOXTECHNIQUE_H
