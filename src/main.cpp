#include <GL/glew.h>

#include <GL/freeglut.h>

#include <memory>

#include "Camera.h"
#include "GLUTBackend.h"
#include "ICallbacks.h"
#include "LightingTechnique.h"
#include "Mesh.h"
#include "Pipeline.h"

#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 720

class Main : public ICallbacks {
    public:
    Main() : mScale(0.0f) {
        mDirectionalLight.color = glm::vec3(1.0f, 1.0f, 1.0f);
        mDirectionalLight.ambientIntensity = 1.0f;
        mDirectionalLight.direction = glm::vec3(1.0f, 0.0f, 1.0f);
        mDirectionalLight.diffuseIntensity = 0.01f;
    }

    ~Main() = default;

    bool init() {
        glm::vec3 pos(-10.0f, 40.0f, 0.0f);
        glm::vec3 target(1.0f, 0.0f, 1.0f);
        glm::vec3 up(0.0f, 1.0f, 0.0f);
        mGameCamera = std::make_unique<Camera>(WINDOW_WIDTH, WINDOW_HEIGHT, pos,
                                               target, up);

        mEffect = std::make_unique<LightingTechnique>(
            "./shaders/light_vertex.glsl", "./shaders/light_fragment.glsl");
        if (!mEffect->init()) return false;

        mEffect->enable();
        mEffect->setTextureUnit(0);

        mMesh = std::make_unique<Mesh>();

        return mMesh->loadMesh("./content/phoenix_ugv.md2");
    }

    void renderSceneCB() override {
        mGameCamera->onRender();

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        mScale += 0.01f;

        Pipeline p;
        p.setScale(0.1f, 0.1f, 0.1f);
        p.setRotation(0.0f, mScale, 0.0f);
        p.setWorldPos(0.0f, 0.0f, 10.0f);
        p.setCamera(mGameCamera->getPos(), mGameCamera->getTarget(),
                    mGameCamera->getUp());
        p.setPerspectiveProj(60.0f, WINDOW_WIDTH, WINDOW_HEIGHT, 1.0f, 100.0f);

        mEffect->setWVP(p.getWVPTransformation());
        mEffect->setWorldMatrix(p.getWorldTransformation());
        mEffect->setDirectionalLight(mDirectionalLight);

        mEffect->setEyeWorldPos(mGameCamera->getPos());
        mEffect->setMatSpecularIntensity(0.0f);
        mEffect->setMatSpecularPower(0.0f);

        mMesh->render();

        glutSwapBuffers();
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
            case 'a':
                mDirectionalLight.ambientIntensity += 0.05f;
                break;
            case 's':
                mDirectionalLight.ambientIntensity -= 0.05f;
                break;
            case 'z':
                mDirectionalLight.diffuseIntensity += 0.05f;
                break;

            case 'x':
                mDirectionalLight.diffuseIntensity -= 0.05f;
                break;
            default:
                break;
        }
    }

    void passiveMouseCB(int x, int y) override { mGameCamera->onMouse(x, y); }

    private:
    std::unique_ptr<Mesh> mMesh;
    std::unique_ptr<Camera> mGameCamera;

    float mScale;

    std::unique_ptr<LightingTechnique> mEffect;
    DirectionLight mDirectionalLight;
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