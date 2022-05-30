#include <GL/glew.h>

#include <GL/freeglut.h>

#include <memory>

#include "billboard/BillboardList.h"
#include "camera/Camera.h"
#include "glut_backend/GLUTBackend.h"
#include "glut_backend/ICallbacks.h"
#include "glut_backend/common.h"
#include "mesh/Mesh.h"
#include "skybox/Skybox.h"
#include "technique/LightingTechnique.h"
#include "utils/Pipeline.h"

#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 720

class Main : public ICallbacks {
    public:
        Main() {
            mDirLight.ambientIntensity = 0.2f;
            mDirLight.diffuseIntensity = 0.8f;
            mDirLight.color = glm::vec3(1.0f, 1.0f, 1.0f);
            mDirLight.direction = glm::vec3(1.0f, 0.0f, 0.0f);

            mPersProjInfo.FOV = 60.0f;
            mPersProjInfo.width = WINDOW_WIDTH;
            mPersProjInfo.height = WINDOW_HEIGHT;
            mPersProjInfo.zNear = 1.0f;
            mPersProjInfo.zFar = 100.0f;
        }

        ~Main() override = default;

        bool init() {
            glm::vec3 pos(0.5f, 1.0f, -1.0f);
            glm::vec3 target(0.0f, -0.5f, 1.0f);
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
            mLightingEffect->setColorTextureUnit(0);
            mLightingEffect->setNormalMapTextureUnit(2);

            mGround = std::make_unique<Mesh>();
            if (!mGround->loadMesh("./content/quad.obj")) {
                return false;
            }

            if (!mBillboardList.init("./content/monster_hellknight.png")) {
                return false;
            }

            mTexture = std::make_unique<Texture>(GL_TEXTURE_2D, "./content/bricks.jpg");
            if (!mTexture->load()) {
                return false;
            }
            mTexture->bind(COLOR_TEXTURE_UNIT);

            mNormalMap =
                std::make_unique<Texture>(GL_TEXTURE_2D, "./content/normal_map.jpg");
            if (!mNormalMap->load()) {
                return false;
            }

            return true;
        }

        void renderSceneCB() override {
            mGameCamera->onRender();

            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            mLightingEffect->enable();

            mTexture->bind(COLOR_TEXTURE_UNIT);
            mNormalMap->bind(NORMAL_TEXTURE_UNIT);

            Pipeline p;
            p.setScale(20.0f, 20.0f, 20.0f);
            p.setRotation(90.0f, 0.0f, 0.0f);
            p.setWorldPos(0.0f, 0.0f, 0.0f);
            p.setCamera(
                mGameCamera->getPos(), mGameCamera->getTarget(), mGameCamera->getUp()
            );
            p.setPerspectiveProj(mPersProjInfo);

            mLightingEffect->setWVP(p.getWVPTransformation());
            mLightingEffect->setWorldMatrix(p.getWorldTransformation());
//            mGround->render();

            mBillboardList.render(p.getWVPTransformation(), mGameCamera->getPos());

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
        PersProjInfo mPersProjInfo;
        DirectionLight mDirLight;
        std::shared_ptr<Camera> mGameCamera;
        std::unique_ptr<LightingTechnique> mLightingEffect;

        BillboardList mBillboardList;

        std::unique_ptr<Mesh> mGround;
        std::unique_ptr<Texture> mTexture;
        std::unique_ptr<Texture> mNormalMap;
};

int main(int argc, char **argv) {
    GLUTBackend::init(argc, argv);

    if (!GLUTBackend::createWindow(WINDOW_WIDTH, WINDOW_HEIGHT, false, "Tutorial 26"))
        return 1;

    auto app = std::make_shared<Main>();

    if (!app->init()) return 1;

    GLUTBackend::run(app);

    return 0;
}