#include "Skybox.h"

#include <iostream>
#include <utility>

#include "../glut_backend/common.h"

Skybox::Skybox(std::shared_ptr<const Camera> camera, const PersProjInfo &persProjInfo)
    : mCamera(std::move(camera)),
      mPersProjInfo(persProjInfo) {}

Skybox::~Skybox() = default;

bool Skybox::init(
    const std::string &directory,
    const std::string &posXFilename,
    const std::string &negXFilename,
    const std::string &posYFilename,
    const std::string &negYFilename,
    const std::string &posZFilename,
    const std::string &negZFilename
) {
    mSkyboxTechnique = std::make_unique<SkyboxTechnique>(
        "./shaders/skybox_vertex.glsl", "./shaders/skybox_fragment.glsl"
    );
    if (!mSkyboxTechnique->init()) {
        std::cerr << "Error initializing the skybox technique" << std::endl;
        return false;
    }

    mSkyboxTechnique->enable();
    mSkyboxTechnique->setTextureUnit(0);

    mCubemapTex = std::make_unique<CubemapTexture>(
        directory,
        posXFilename,
        negXFilename,
        posYFilename,
        negYFilename,
        posZFilename,
        negZFilename
    );
    if (!mCubemapTex->load()) {
        return false;
    }

    mMesh = std::make_unique<Mesh>();

    return mMesh->loadMesh("./content/sphere.obj");
}

void Skybox::render() {
    mSkyboxTechnique->enable();

    GLint oldDepthFuncMode;
    glGetIntegerv(GL_DEPTH_FUNC, &oldDepthFuncMode);

    glDepthFunc(GL_LEQUAL);
    glDepthMask(GL_FALSE);

    Pipeline p;
    p.setScale(20.0f, 20.0f, 20.0f);
    p.setRotation(0.0f, 0.0f, 0.0f);
    p.setWorldPos(mCamera->getPos());
    p.setCamera(mCamera->getPos(), mCamera->getTarget(), mCamera->getUp());
    p.setPerspectiveProj(mPersProjInfo);
    mSkyboxTechnique->setWVP(p.getWVPTransformation());
    mCubemapTex->bind(COLOR_TEXTURE_UNIT);
    mMesh->render();

    glDepthFunc(oldDepthFuncMode);
    glDepthMask(GL_TRUE);
}
