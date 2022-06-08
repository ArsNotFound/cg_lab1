#include "ParticleSystem.h"

#include "../glut_backend/common.h"
#include "../utils/utils.h"

ParticleSystem::ParticleSystem()
    : mCurrVB(0),
      mCurrTFB(1),
      mIsFirst(true),
      mTime(0),
      mUpdateTechnique(
          "./shaders/particle_vertex.glsl", "./shaders/particle_geometry.glsl"
      ),
      mBillboardTechnique(
          "./shaders/billboard_vertex.glsl",
          "./shaders/billboard_geometry.glsl",
          "./shaders/billboard_fragment.glsl"
      ) {
    mParticleBuffer.resize(2);
    mTransformFeedback.resize(2);
}

ParticleSystem::~ParticleSystem() {
    if (mTransformFeedback[0] != 0)
        glDeleteTransformFeedbacks(2, &mTransformFeedback.front());

    if (mParticleBuffer[0] != 0) glDeleteBuffers(2, &mParticleBuffer.front());
}

bool ParticleSystem::init(const glm::vec3 &pos) {
    std::vector<Particle> particles(MAX_PARTICLES);

    particles[0].type = PARTICLE_TYPE_LAUNCHER;
    particles[0].pos = pos;
    particles[0].vel = glm::vec3(0.0f, 0.0001f, 0.0f);
    particles[0].lifetimeMillis = 0.0f;

    glGenTransformFeedbacks(2, &mTransformFeedback.front());
    glGenBuffers(2, &mParticleBuffer.front());

    for (auto i = 0; i < 2; i++) {
        glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, mTransformFeedback[i]);
        glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, mParticleBuffer[i]);
        glBindBuffer(GL_ARRAY_BUFFER, mParticleBuffer[i]);
        glBufferDataVector(GL_ARRAY_BUFFER, particles, GL_DYNAMIC_DRAW);
    }

    if (!mUpdateTechnique.init()) return false;
    mUpdateTechnique.enable();
    mUpdateTechnique.setRandomTextureUnit(RANDOM_TEXTURE_UNIT_INDEX);
    mUpdateTechnique.setLauncherLifetime(100.0f);
    mUpdateTechnique.setShellLifetime(10000.0f);
    mUpdateTechnique.setSecondaryShellLifetime(2500.0f);

    if (!mRandomTexture.init(1000)) return false;
    mRandomTexture.bind(RANDOM_TEXTURE_UNIT);

    if (!mBillboardTechnique.init()) return false;
    mBillboardTechnique.enable();
    mBillboardTechnique.setColorTextureUnit(COLOR_TEXTURE_UNIT_INDEX);
    mBillboardTechnique.setBillboardSize(0.01f);
    GLExitIfError();

    mTexture = std::make_unique<Texture>(GL_TEXTURE_2D, "./content/fireworks_red.jpg");
    if (!mTexture->load()) return false;

    return GLCheckError();
}

void ParticleSystem::render(
    int deltaTimeMillis, const glm::mat4 &vp, const glm::vec3& cameraPos
) {
    mTime += deltaTimeMillis;

    updateParticles(deltaTimeMillis);

    renderParticles(vp, cameraPos);

    mCurrVB = mCurrTFB;
    mCurrTFB = (mCurrTFB + 1) & 0x1;
}

void ParticleSystem::updateParticles(int deltaTimeMillis) {
    mUpdateTechnique.enable();
    mUpdateTechnique.setTime(mTime);
    mUpdateTechnique.setDeltaTimeMillis(static_cast<float>(deltaTimeMillis));

    mRandomTexture.bind(RANDOM_TEXTURE_UNIT);

    glEnable(GL_RASTERIZER_DISCARD);

    glBindBuffer(GL_ARRAY_BUFFER, mParticleBuffer[mCurrVB]);
    glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, mTransformFeedback[mCurrTFB]);

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
    glEnableVertexAttribArray(3);

    glVertexAttribPointer(
        0,
        1,
        GL_FLOAT,
        GL_FALSE,
        sizeof(Particle),
        reinterpret_cast<const GLvoid *>(PARTICLE_TYPE_OFFSET)
    );
    glVertexAttribPointer(
        1,
        3,
        GL_FLOAT,
        GL_FALSE,
        sizeof(Particle),
        reinterpret_cast<const GLvoid *>(PARTICLE_POS_OFFSET)
    );
    glVertexAttribPointer(
        2,
        3,
        GL_FLOAT,
        GL_FALSE,
        sizeof(Particle),
        reinterpret_cast<const GLvoid *>(PARTICLE_VEL_OFFSET)
    );
    glVertexAttribPointer(
        3,
        1,
        GL_FLOAT,
        GL_FALSE,
        sizeof(Particle),
        reinterpret_cast<const GLvoid *>(PARTICLE_LIFETIME_OFFSET)
    );

    glBeginTransformFeedback(GL_POINTS);

    if (mIsFirst) {
        glDrawArrays(GL_POINTS, 0, 1);
        mIsFirst = false;
    } else {
        glDrawTransformFeedback(GL_POINTS, mTransformFeedback[mCurrVB]);
    }

    glEndTransformFeedback();

    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(2);
    glDisableVertexAttribArray(3);
}

void ParticleSystem::renderParticles(const glm::mat4 &vp, const glm::vec3 &cameraPos) {
    mBillboardTechnique.enable();
    mBillboardTechnique.setCameraPosition(cameraPos);
    mBillboardTechnique.setVP(vp);
    mTexture->bind(COLOR_TEXTURE_UNIT);

    glDisable(GL_RASTERIZER_DISCARD);

    glBindBuffer(GL_ARRAY_BUFFER, mParticleBuffer[mCurrTFB]);

    glEnableVertexAttribArray(0);

    glVertexAttribPointer(
        0,
        3,
        GL_FLOAT,
        GL_FALSE,
        sizeof(Particle),
        reinterpret_cast<const GLvoid *>(PARTICLE_POS_OFFSET)
    );

    glDrawTransformFeedback(GL_POINTS, mTransformFeedback[mCurrTFB]);

    glDisableVertexAttribArray(0);
}