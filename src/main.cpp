#include <GL/glew.h>

#include <GL/freeglut.h>

#include <memory>

#include "camera/Camera.h"
#include "fbo/ShadowMapFBO.h"
#include "glut_backend/GLUTBackend.h"
#include "glut_backend/ICallbacks.h"
#include "mesh/Mesh.h"
#include "technique/LightingTechnique.h"
#include "technique/ShadowMapTechnique.h"
#include "utils/Pipeline.h"

#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 720

class Main : public ICallbacks {
    public:
        Main()
            : mScale(0.0f) {
            mSpotLight.ambientIntensity = 0.1f;
            mSpotLight.diffuseIntensity = 0.9f;
            mSpotLight.color = glm::vec3(1.0f, 1.0f, 1.0f);
            mSpotLight.attenuation.linear = 0.01f;
            mSpotLight.position = glm::vec3(-20.0, 20.0, 1.0f);
            mSpotLight.direction = glm::vec3(1.0f, -1.0f, 0.0f);
            mSpotLight.cutoff = 20.0f;
        }

        ~Main() override = default;

        bool init() {
            glm::vec3 pos(3.0f, 8.0f, -10.0f);
            glm::vec3 target(0.0f, -0.2f, 1.0f);
            glm::vec3 up(0.0f, 1.0f, 0.0f);

            if (!mShadowMapFBO.init(WINDOW_WIDTH, WINDOW_HEIGHT)) {
                return false;
            }

            mGameCamera =
                std::make_unique<Camera>(WINDOW_WIDTH, WINDOW_HEIGHT, pos, target, up);

            mLightingEffect = std::make_unique<LightingTechnique>(
                "./shaders/light_vertex.glsl", "./shaders/light_fragment.glsl"
            );

            if (!mLightingEffect->init()) {
                std::cerr << "Error initializing the lighting technique" << std::endl;
                return false;
            }

            mLightingEffect->enable();
            mLightingEffect->setSpotLights({mSpotLight});
            mLightingEffect->setTextureUnit(0);
            mLightingEffect->setShadowMapTextureUnit(1);

            mShadowMapEffect = std::make_unique<ShadowMapTechnique>(
                "./shaders/shadow_vertex.glsl", "./shaders/shadow_fragment.glsl"
            );
            if (!mShadowMapEffect->init()) {
                std::cerr << "Error initializing the shadow map technique" << std::endl;
                return false;
            }

            mQuad = std::make_unique<Mesh>();
            if (!mQuad->loadMesh("./content/quad.obj")) {
                return false;
            }

            mGroundTexture =
                std::make_unique<Texture>(GL_TEXTURE_2D, "./content/test.png");
            if (!mGroundTexture->load()) {
                return false;
            }

            mMesh = std::make_unique<Mesh>();
            return mMesh->loadMesh("./content/phoenix_ugv.md2");
        }

        void renderSceneCB() override {
            mGameCamera->onRender();
            mScale += 0.5f;

            ShadowMapPass();
            RenderPass();

            glutSwapBuffers();
        }

        virtual void ShadowMapPass() {
            mShadowMapFBO.bindForWriting();

            glClear(GL_DEPTH_BUFFER_BIT);

            mShadowMapEffect->enable();

            Pipeline p;
            p.setScale(0.1f, 0.1f, 0.1f);
            p.setRotation(0.0f, mScale, 0.0f);
            p.setWorldPos(0.0f, 0.0f, 5.0f);
            p.setCamera(
                mSpotLight.position, mSpotLight.direction, glm::vec3(0.0f, 1.0f, 0.0f)
            );
            p.setPerspectiveProj(60.0f, WINDOW_WIDTH, WINDOW_HEIGHT, 1.0f, 50.0f);
            mShadowMapEffect->setWVP(p.getWVPTransformation());
            mMesh->render();

            glBindFramebuffer(GL_FRAMEBUFFER, 0);
        }

        virtual void RenderPass() {
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            mLightingEffect->enable();

            mShadowMapFBO.bindForReading(GL_TEXTURE1);

            Pipeline p;
            p.setPerspectiveProj(60.0f, WINDOW_WIDTH, WINDOW_HEIGHT, 1.0f, 50.0f);
            p.setScale(10.0f, 10.0f, 10.0f);
            p.setWorldPos(0.0f, 0.0f, 1.0f);
            p.setRotation(90.0f, 0.0f, 0.0f);
            p.setCamera(
                mGameCamera->getPos(), mGameCamera->getTarget(), mGameCamera->getUp()
            );
            mLightingEffect->setWVP(p.getWVPTransformation());
            mLightingEffect->setWorldMatrix(p.getWorldTransformation());

            p.setCamera(
                mSpotLight.position, mSpotLight.direction, glm::vec3(0.0f, 1.0f, 0.0f)
            );
            mLightingEffect->setLightWVP(p.getWVPTransformation());
            mLightingEffect->setEyeWorldPos(mGameCamera->getPos());
            mGroundTexture->bind(GL_TEXTURE0);
            mQuad->render();

            p.setScale(0.1f, 0.1f, 0.1f);
            p.setRotation(0.0f, mScale, 0.0f);
            p.setWorldPos(0.0f, 0.0f, 3.0f);
            p.setCamera(
                mGameCamera->getPos(), mGameCamera->getTarget(), mGameCamera->getUp()
            );
            mLightingEffect->setWVP(p.getWVPTransformation());
            mLightingEffect->setWorldMatrix(p.getWorldTransformation());

            p.setCamera(
                mSpotLight.position, mSpotLight.direction, glm::vec3(0.0f, 1.0f, 0.0f)
            );
            mLightingEffect->setLightWVP(p.getWVPTransformation());

            mMesh->render();
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
        std::unique_ptr<Texture> mGroundTexture;

        std::unique_ptr<LightingTechnique> mLightingEffect;
        std::unique_ptr<ShadowMapTechnique> mShadowMapEffect;
        std::unique_ptr<Camera> mGameCamera;

        ShadowMapFBO mShadowMapFBO;
        SpotLight mSpotLight;
};

int main(int argc, char **argv) {
    GLUTBackend::init(argc, argv);

    if (!GLUTBackend::createWindow(WINDOW_WIDTH, WINDOW_HEIGHT, true, "Tutorial 24"))
        return 1;

    auto app = std::make_shared<Main>();

    if (!app->init()) return 1;

    GLUTBackend::run(app);

    return 0;
}