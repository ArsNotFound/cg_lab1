#include <GL/glew.h>

#include <GL/freeglut.h>

#include <memory>

#include <sys/time.h>

#include "billboard/BillboardList.h"
#include "camera/Camera.h"
#include "glut_backend/GLUTBackend.h"
#include "glut_backend/ICallbacks.h"
#include "glut_backend/common.h"
#include "light/LightingTechnique.h"
#include "mesh/Mesh.h"
#include "particle_system/ParticleSystem.h"
#include "utils/Pipeline.h"

#define WINDOW_WIDTH 1920
#define WINDOW_HEIGHT 1080

long long getCurrentTimeMillis() {
    timeval t{};
    gettimeofday(&t, nullptr);

    long long ret = t.tv_sec * 1000 + t.tv_usec / 1000;
    return ret;
}

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

            mCurrentTimeMillis = getCurrentTimeMillis();
        }

        ~Main() override = default;

        bool init() {
            glm::vec3 pos(0.0f, 0.4f, -0.5f);
            glm::vec3 target(0.0f, 0.0f, 1.0f);
            glm::vec3 up(0.0f, 1.0f, 0.0f);

            mGameCamera =
                std::make_shared<Camera>(WINDOW_WIDTH, WINDOW_HEIGHT, pos, target, up);

            mLightingTechnique = std::make_unique<LightingTechnique>(
                "./shaders/light_vertex.glsl", "./shaders/light_fragment.glsl"
            );
            if (!mLightingTechnique->init()) {
                std::cerr << "Error initializing the lighting technique" << std::endl;
                return false;
            }
            mLightingTechnique->enable();
            mLightingTechnique->setDirectionalLight(mDirLight);
            mLightingTechnique->setColorTextureUnit(COLOR_TEXTURE_UNIT_INDEX);
            mLightingTechnique->setNormalMapTextureUnit(NORMAL_TEXTURE_UNIT_INDEX);

            mGround = std::make_unique<Mesh>();
            if (!mGround->loadMesh("./content/quad.obj")) {
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

            return mParticleSystem.init(glm::vec3(0.0f, 0.0f, 1.0f));
        }

        void renderSceneCB() override {
            long long timeNowMillis = getCurrentTimeMillis();
            assert(timeNowMillis >= mCurrentTimeMillis);
            auto deltaTimeMillis =
                static_cast<int>(timeNowMillis - mCurrentTimeMillis);
            mGameCamera->onRender();

            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            mLightingTechnique->enable();

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

            mLightingTechnique->setWVP(p.getWVPTransformation());
            mLightingTechnique->setWorldMatrix(p.getWorldTransformation());

            mGround->render();

            mParticleSystem.render(
                deltaTimeMillis, p.getVPTransformation(), mGameCamera->getPos()
            );

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
        long long mCurrentTimeMillis;

        std::unique_ptr<LightingTechnique> mLightingTechnique;
        std::shared_ptr<Camera> mGameCamera;

        DirectionLight mDirLight;

        std::unique_ptr<Mesh> mGround;
        std::unique_ptr<Texture> mTexture;
        std::unique_ptr<Texture> mNormalMap;

        PersProjInfo mPersProjInfo{};

        ParticleSystem mParticleSystem;
};

int main(int argc, char **argv) {
    GLUTBackend::init(argc, argv);

    if (!GLUTBackend::createWindow(WINDOW_WIDTH, WINDOW_HEIGHT, true, "Tutorial 26"))
        return 1;

    auto app = std::make_shared<Main>();

    if (!app->init()) return 1;

    GLUTBackend::run(app);

    return 0;
}