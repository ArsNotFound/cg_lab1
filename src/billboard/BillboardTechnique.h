#ifndef CG_LAB1_BILLBOARDTECHNIQUE_H
#define CG_LAB1_BILLBOARDTECHNIQUE_H

#include <string>

#include <glm/glm.hpp>

#include "../technique/Technique.h"
class BillboardTechnique : public Technique {
    public:
        BillboardTechnique(
            std::string vertexShaderFilename,
            std::string geometryShaderFilename,
            std::string fragmentShaderFilename
        );

        bool init() override;

        void setVP(const glm::mat4& vp) const;

        void setCameraPosition(const glm::vec3& pos) const;

        void setColorTextureUnit(int textureUnit) const;

    private:
        std::string mVertexShaderFilename;
        std::string mGeometryShaderFilename;
        std::string mFragmentShaderFilename;

        GLint mVPLocation;
        GLint mCameraPosLocation;
        GLint mColorMapLocation;
};

#endif  // CG_LAB1_BILLBOARDTECHNIQUE_H
