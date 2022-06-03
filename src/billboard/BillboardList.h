#ifndef CG_LAB1_BILLBOARDLIST_H
#define CG_LAB1_BILLBOARDLIST_H

#include <GL/glew.h>

#include <memory>
#include <string>

#include <glm/glm.hpp>

#include "../texture/Texture.h"
#include "BillboardTechnique.h"

class BillboardList {
    public:
        BillboardList();
        ~BillboardList();

        bool init(const std::string& texFilename);

        void render(const glm::mat4& vp, const glm::vec3& cameraPos);

    private:
        void createPositionBuffer();

        GLuint mVA;
        GLuint mVB;
        std::unique_ptr<Texture> mTexture;
        BillboardTechnique mTechnique;
};

#endif  // CG_LAB1_BILLBOARDLIST_H
