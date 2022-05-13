#include <GL/glew.h>
#include <GL/freeglut.h>
#include <iostream>
#include <memory>

#include "GLUTBackend.h"
#include "ICallbacks.h"
#include "LightingTechnique.h"
#include "Texture.h"
#include "Camera.h"
#include "Vertex.h"
#include "Pipeline.h"
#include "utils.h"

#include <glm/gtx/string_cast.hpp>

#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 720

class Main : public ICallbacks {
public:
    Main() : mScale(0.0f) {
        mDirectionalLight.color = glm::vec3(1.0f, 1.0f, 1.0f);
        mDirectionalLight.ambientIntensity = -0.1f;
        mDirectionalLight.direction = glm::vec3(1.0f, 0.0f, 0.0f);
        mDirectionalLight.diffuseIntensity = 0.0f;
    }

    ~Main() = default;

    bool init() {
        glm::vec3 pos(0.0f, 0.0f, 0.0f);
        glm::vec3 target(0.0f, 0.0f, 1.0f);
        glm::vec3 up(0.0f, 1.0f, 0.0f);
        mGameCamera = std::make_unique<Camera>(WINDOW_WIDTH, WINDOW_HEIGHT, pos, target, up);

        vertices = {
                Vertex(glm::vec3(-10.0f, -2.0f, -10.0f), glm::vec2(0.0f, 0.0f)),
                Vertex(glm::vec3(10.0f, -2.0f, -10.0f), glm::vec2(1.0f, 0.0f)),
                Vertex(glm::vec3(10.0f, -2.0f, 10.0f), glm::vec2(1.0f, 1.0f)),
                Vertex(glm::vec3(-10.0f, -2.0f, 10.0f), glm::vec2(0.0f, 1.0f))
        };

        indices = {
                0, 2, 1,
                0, 3, 2
        };

        calcNormals(indices, vertices);

        createBuffers();

        mEffect = std::make_unique<LightingTechnique>(
                "./shaders/vertex.glsl",
                "./shaders/fragment.glsl"
        );
        if (!mEffect->init())
            return false;

        mEffect->enable();
        mEffect->setTextureUnit(0);

        mTexture = std::make_unique<Texture>(GL_TEXTURE_2D, "./content/test.png");
        if (!mTexture->load())
            return false;

        return true;
    }

    void renderSceneCB() override {
        mGameCamera->onRender();

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        mScale += 0.1f;
//
//        std::vector<PointLight> pl(3);
//        pl[0].diffuseIntensity = 0.5f;
//        pl[0].color = glm::vec3(1.0f, 0.0f, 0.0f);
//        pl[0].position = glm::vec3(sinf(mScale) * 10, 1.0f, cosf(mScale) * 10);
//        pl[0].attenuation.linear = 0.1f;
//
//        pl[1].diffuseIntensity = 0.5f;
//        pl[1].color = glm::vec3(0.0f, 1.0f, 0.0f);
//        pl[1].position = glm::vec3(sinf(mScale + 2.1f) * 10, 1.0f, cosf(mScale + 2.1f) * 10);
//        pl[1].attenuation.linear = 0.1f;
//
//        pl[2].diffuseIntensity = 0.5f;
//        pl[2].color = glm::vec3(0.0f, 0.0f, 1.0f);
//        pl[2].position = glm::vec3(sinf(mScale + 4.2f) * 10, 1.0f, cosf(mScale + 4.2f) * 10);
//        pl[2].attenuation.linear = 0.1f;
//
//        mEffect->setPointLights(pl);

        std::vector<SpotLight> sl(2);

        sl[0].diffuseIntensity = 15.0f;
        sl[0].color = glm::vec3(1.0f, 1.0f, 0.7f);
        sl[0].position = glm::vec3(-0.0f, -1.9f, -0.0f);
        sl[0].direction = glm::vec3(sinf(mScale), 0.0f, cosf(mScale));
        sl[0].attenuation.linear = 0.1f;
        sl[0].cutoff = 20.0f;

        sl[1].diffuseIntensity = 5.0f;
        sl[1].color = glm::vec3(0.0f, 1.0f, 1.0f);
        sl[1].position = mGameCamera->getPos();
        sl[1].direction = mGameCamera->getTarget();
        sl[1].attenuation.linear = 0.1f;
        sl[1].cutoff = 10.0f;

        mEffect->setSpotLights(sl);

        Pipeline p;
        p.setRotation(0.0f, 0.0f, 0.0f);
        p.setWorldPos(0.0f, 0.0f, 1.0f);
        p.setCamera(mGameCamera->getPos(), mGameCamera->getTarget(), mGameCamera->getUp());
        p.setPerspectiveProj(60.0f, WINDOW_WIDTH, WINDOW_HEIGHT, 0.01f, 100.0f);

        const auto wvpTransformation = p.getWVPTransformation();

//        static auto prevWVP = glm::mat4(0.0f);
//        if (wvpTransformation != prevWVP) {
//            std::cout << glm::to_string(wvpTransformation) << std::endl;
//            prevWVP = wvpTransformation;
//        }

        const auto worldTransformation = p.getWorldTransformation();

        static auto prevWorld = glm::mat4(0.0f);
        if (worldTransformation != prevWorld) {
            std::cout << glm::to_string(worldTransformation) << std::endl;
            prevWorld = worldTransformation;
        }

        mEffect->setWVP(wvpTransformation);
        mEffect->setWorldMatrix(worldTransformation);
        mEffect->setDirectionalLight(mDirectionalLight);

        mEffect->setEyeWorldPos(mGameCamera->getPos());
        mEffect->setMatSpecularIntensity(1.0f);
        mEffect->setMatSpecularPower(32);

        mTexture->bind(GL_TEXTURE0);

        glBindVertexArray(mVAO);
//        glDrawElements(GL_TRIANGLE_FAN, 6, GL_UNSIGNED_INT, nullptr);
        glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
        glBindVertexArray(0);

        glutSwapBuffers();
    }

    void idleCB() override {
        renderSceneCB();
    }

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

    void passiveMouseCB(int x, int y) override {
        mGameCamera->onMouse(x, y);
    }

private:
    GLuint mVAO;
    GLuint mVBO;
    GLuint mIBO;
    std::vector<Vertex> vertices;
    std::vector<GLuint> indices;

    std::unique_ptr<Texture> mTexture;
    std::unique_ptr<Camera> mGameCamera;

    float mScale;

    std::unique_ptr<LightingTechnique> mEffect;
    DirectionLight mDirectionalLight;

    void createBuffers() {
        glGenVertexArrays(1, &mVAO);
        glGenBuffers(1, &mVBO);
        glGenBuffers(1, &mIBO);

        glBindVertexArray(mVAO);

        glBindBuffer(GL_ARRAY_BUFFER, mVBO);
        glBufferDataVector(GL_ARRAY_BUFFER, vertices, GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIBO);
        glBufferDataVector(GL_ELEMENT_ARRAY_BUFFER, indices, GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<GLvoid *>(VERTEX_POS_OFFSET));
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<GLvoid *>(VERTEX_TEX_OFFSET));
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                              reinterpret_cast<GLvoid *>(VERTEX_NORMAL_OFFSET));
        glEnableVertexAttribArray(2);

        glBindVertexArray(0);
    }

    static void calcNormals(const std::vector<GLuint> &indices, std::vector<Vertex> &vertices) {
        for (auto i = 0; i < indices.size(); i += 3) {
            auto index0 = indices[i];
            auto index1 = indices[i + 1];
            auto index2 = indices[i + 2];
            glm::vec3 v1 = vertices[index1].mPos - vertices[index0].mPos;
            glm::vec3 v2 = vertices[index2].mPos - vertices[index0].mPos;
            glm::vec3 normal = glm::normalize(glm::cross(v1, v2));

            vertices[index0].mNormal += normal;
            vertices[index1].mNormal += normal;
            vertices[index2].mNormal += normal;
        }

        for (auto &vertex: vertices) {
            vertex.mNormal = glm::normalize(vertex.mNormal);
        }
    }
};

int main(int argc, char **argv) {
    GLUTBackend::init(argc, argv);

    if (!GLUTBackend::createWindow(WINDOW_WIDTH, WINDOW_HEIGHT, false, "Tutorial 17"))
        return 1;

    auto app = std::make_shared<Main>();

    if (!app->init())
        return 1;

    GLUTBackend::run(app);

    return 0;
}