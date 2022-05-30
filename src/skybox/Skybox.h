#ifndef CG_LAB1_SKYBOX_H
#define CG_LAB1_SKYBOX_H

#include <memory>
#include <string>

#include "../camera/Camera.h"
#include "../mesh/Mesh.h"
#include "../technique/SkyboxTechnique.h"
#include "../texture/CubemapTexture.h"
#include "../utils/Pipeline.h"

class Skybox {
    public:
        Skybox(std::shared_ptr<const Camera> camera, const PersProjInfo& persProjInfo);

        ~Skybox();

        bool init(const std::string& directory,
                  const std::string& posXFilename,
                  const std::string& negXFilename,
                  const std::string& posYFilename,
                  const std::string& negYFilename,
                  const std::string& posZFilename,
                  const std::string& negZFilename);

        void render();

    private:
        std::unique_ptr<SkyboxTechnique> mSkyboxTechnique;
        std::shared_ptr<const Camera> mCamera;
        std::unique_ptr<CubemapTexture> mCubemapTex;
        std::unique_ptr<Mesh> mMesh;
        PersProjInfo mPersProjInfo;
};

#endif  // CG_LAB1_SKYBOX_H
