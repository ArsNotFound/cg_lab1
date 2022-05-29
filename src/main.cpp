#include <GL/glew.h>

#include <GL/freeglut.h>

#include <memory>

#include "Camera.h"
#include "GLUTBackend.h"
#include "ICallbacks.h"
#include "LightingTechnique.h"
#include "Mesh.h"
#include "Pipeline.h"
#include "ShadowMapFBO.h"
#include "ShadowMapTechnique.h"

#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 720

class Main : public ICallbacks {
    public:
    Main() : mScale(0.0f) {
        mSpotLight.ambientIntensity = 0.0f;
        mSpotLight.diffuseIntensity = 0.9f;
        mSpotLight.color = glm::vec3(1.0f, 1.0f, 1.0f);
        mSpotLight.attenuation.linear = 0.01f;
        mSpotLight.position = glm::vec3(-20.0, 20.0, 5.0f);
        mSpotLight.direction = glm::vec3(1.0f, -1.0f, 0.0f);
        mSpotLight.cutoff = 20.0f;
    }

    ~Main() override = default;

    bool init() {
        glm::vec3 pos(-10.0f, 40.0f, 0.0f);
        glm::vec3 target(1.0f, 0.0f, 1.0f);
        glm::vec3 up(0.0f, 1.0f, 0.0f);
        mGameCamera = std::make_unique<Camera>(WINDOW_WIDTH, WINDOW_HEIGHT, pos,
                                               target, up);

        mEffect = std::make_unique<LightingTechnique>(
            "./shaders/light_vertex.glsl", "./shaders/light_fragment.glsl");
        if (!mEffect->init()) {
            std::cerr << "Error initializing the lighting technique"
                      << std::endl;
            return false;
        }

        mShadowMapTech = std::make_unique<ShadowMapTechnique>(
            "./shaders/shadow_vertex.glsl", "./shaders/shadow_fragment.glsl");
        if (!mShadowMapTech->init()) {
            std::cerr << "Error initializing the shadow map technique"
                      << std::endl;
            return false;
        }

        mShadowMapTech->enable();

        mQuad = std::make_unique<Mesh>();

        if (!mQuad->loadMesh("./content/quad.obj")) {
            return false;
        }

        mMesh = std::make_unique<Mesh>();

        return mMesh->loadMesh("./content/phoenix_ugv.md2");
    }

    void renderSceneCB() override {
        mGameCamera->onRender();
        mScale += 0.05f;

        ShadowMapPass();
        RenderPass();

        glutSwapBuffers();
    }

    virtual void ShadowMapPass() {
        mShadowMapFBO.bindForWriting();

        glClear(GL_DEPTH_BUFFER_BIT);

        Pipeline p;
        p.setScale(0.2f, 0.2f, 0.2f);
        p.setRotation(0.1f, mScale, 0.1f);
        p.setWorldPos(0.0f, 0.0f, 5.0f);
        p.setCamera(mSpotLight.position, mSpotLight.direction,
                    glm::vec3(0.0f, 1.0f, 0.0f));
        p.setPerspectiveProj(60.0f, WINDOW_WIDTH, WINDOW_HEIGHT, 1.0f, 50.0f);
        mShadowMapTech->setWVP(p.getWVPTransformation());
        mMesh->render();

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    virtual void RenderPass() {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        mShadowMapTech->setTextureUnit(0);
        mShadowMapFBO.bindForReading(GL_TEXTURE0);

        Pipeline p;
        p.setScale(5.0f, 5.0f, 5.0f);
        p.setWorldPos(0.0f, 0.0f, 10.0f);
        p.setCamera(mGameCamera->getPos(), mGameCamera->getTarget(),
                    mGameCamera->getUp());
        p.setPerspectiveProj(60.0f, WINDOW_WIDTH, WINDOW_HEIGHT, 1.0f, 50.0f);
        mShadowMapTech->setWVP(p.getWVPTransformation());
        mQuad->render();
    }

    void idleCB() override { renderSceneCB(); }

    void specialKeyboardCB(int key, int x, int y) override {
        mGameCamera->onKeyPressed(key);
    }

    void keyboardCB(unsigned char key, int x, int y) override {
        switch (key) {
            case 'q':
                glutLeaveMainLoop();
                break;
            default:
                break;
        }
    }

    void passiveMouseCB(int x, int y) override { mGameCamera->onMouse(x, y); }

    private:
    float mScale;

    std::unique_ptr<Mesh> mMesh;
    std::unique_ptr<Mesh> mQuad;

    std::unique_ptr<LightingTechnique> mEffect;
    std::unique_ptr<ShadowMapTechnique> mShadowMapTech;
    std::unique_ptr<Camera> mGameCamera;

    ShadowMapFBO mShadowMapFBO;
    SpotLight mSpotLight;
};

int main(int argc, char **argv) {
    GLUTBackend::init(argc, argv);

    if (!GLUTBackend::createWindow(WINDOW_WIDTH, WINDOW_HEIGHT, false,
                                   "Tutorial 17"))
        return 1;

    auto app = std::make_shared<Main>();

    if (!app->init()) return 1;

    GLUTBackend::run(app);

    return 0;
}