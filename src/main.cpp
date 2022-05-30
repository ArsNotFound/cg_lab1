#include <GL/glew.h>

#include <GL/freeglut.h>

#include <memory>

#include "camera/Camera.h"
#include "glut_backend/GLUTBackend.h"
#include "glut_backend/ICallbacks.h"
#include "mesh/Mesh.h"
#include "skybox/Skybox.h"
#include "technique/LightingTechnique.h"
#include "utils/Pipeline.h"

#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 720

class Main : public ICallbacks {
    public:
        Main() {
            mScale = 0.0f;

            mDirLight.ambientIntensity = 0.2f;
            mDirLight.diffuseIntensity = 0.8f;
            mDirLight.color = glm::vec3(1.0f, 1.0f, 1.0f);
            mDirLight.direction = glm::vec3(1.0f, -1.0f, 0.0f);

            mPersProjInfo.FOV = 60.0f;
            mPersProjInfo.width = WINDOW_WIDTH;
            mPersProjInfo.height = WINDOW_HEIGHT;
            mPersProjInfo.zNear = 1.0f;
            mPersProjInfo.zFar = 100.0f;
        }

        ~Main() override = default;

        bool init() {
            glm::vec3 pos(0.0f, 1.0f, -20.0f);
            glm::vec3 target(0.0f, 0.0f, 1.0f);
            glm::vec3 up(0.0f, 1.0f, 0.0f);

            mGameCamera =
                std::make_shared<Camera>(WINDOW_WIDTH, WINDOW_HEIGHT, pos, target, up);

            mLightingEffect = std::make_unique<LightingTechnique>(
                "./shaders/light_vertex.glsl", "./shaders/light_fragment.glsl"
            );

            if (!mLightingEffect->init()) {
                std::cerr << "Error initializing the lighting technique" << std::endl;
                return false;
            }

            mLightingEffect->enable();
            mLightingEffect->setDirectionalLight(mDirLight);
            mLightingEffect->setTextureUnit(0);

            mTankMesh = std::make_unique<Mesh>();
            if (!mTankMesh->loadMesh("./content/phoenix_ugv.md2")) {
                return false;
            }

            mSkybox = std::make_unique<Skybox>(mGameCamera, mPersProjInfo);

            if (!mSkybox->init(
                    "./content/",
                    "sp3right.jpg",
                    "sp3left.jpg",
                    "sp3top.jpg",
                    "sp3bot.jpg",
                    "sp3front.jpg",
                    "sp3back.jpg"
                )) {
                return false;
            }

            return true;
        }

        void renderSceneCB() override {
            mGameCamera->onRender();
            mScale += 0.05f;

            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            mLightingEffect->enable();

            Pipeline p;
            p.setScale(0.1f, 0.1f, 0.1f);
            p.setRotation(0.0f, mScale, 0.0f);
            p.setWorldPos(0.0f, -5.0f, 3.0f);
            p.setCamera(
                mGameCamera->getPos(), mGameCamera->getTarget(), mGameCamera->getUp()
            );
            p.setPerspectiveProj(mPersProjInfo);

            mLightingEffect->setWVP(p.getWVPTransformation());
            mLightingEffect->setWorldMatrix(p.getWorldTransformation());

            mTankMesh->render();
            mSkybox->render();

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
                default:
                    break;
            }
        }

        void passiveMouseCB(int x, int y) override { mGameCamera->onMouse(x, y); }

    private:
        float mScale;
        PersProjInfo mPersProjInfo;
        DirectionLight mDirLight;
        std::shared_ptr<Camera> mGameCamera;
        std::unique_ptr<LightingTechnique> mLightingEffect;

        std::unique_ptr<Mesh> mTankMesh;
        std::unique_ptr<Skybox> mSkybox;
};

int main(int argc, char **argv) {
    GLUTBackend::init(argc, argv);

    if (!GLUTBackend::createWindow(WINDOW_WIDTH, WINDOW_HEIGHT, false, "Tutorial 24"))
        return 1;

    auto app = std::make_shared<Main>();

    if (!app->init()) return 1;

    GLUTBackend::run(app);

    return 0;
}