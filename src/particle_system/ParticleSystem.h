#ifndef CG_LAB1_PARTICLESYSTEM_H
#define CG_LAB1_PARTICLESYSTEM_H

#include <GL/glew.h>

#include <glm/glm.hpp>

#include "../billboard/BillboardTechnique.h"
#include "../texture/Texture.h"
#include "PSUpdateTechnique.h"
#include "RandomTexture.h"

#define MAX_PARTICLES 1000
#define PARTICLE_LIFETIME 10.0f

#define PARTICLE_TYPE_LAUNCHER 0.0f
#define PARTICLE_TYPE_SHELL 1.0f
#define PARTICLE_TYPE_SECONDARY_SHELL 2.0f

#define PARTICLE_TYPE_OFFSET 0
#define PARTICLE_POS_OFFSET (PARTICLE_TYPE_OFFSET + sizeof(float))
#define PARTICLE_VEL_OFFSET (PARTICLE_POS_OFFSET + sizeof(glm::vec3))
#define PARTICLE_LIFETIME_OFFSET (PARTICLE_VEL_OFFSET + sizeof(glm::vec3))

struct Particle {
        float type;
        glm::vec3 pos;
        glm::vec3 vel;
        float lifetimeMillis;
};

class ParticleSystem {
    public:
        ParticleSystem();

        ~ParticleSystem();

        bool init(const glm::vec3& pos);

        void render(int deltaTimeMillis, const glm::mat4& vp, const glm::vec3& cameraPos);

    private:
        void updateParticles(int deltaTimeMillis);
        void renderParticles(const glm::mat4& vp, const glm::vec3& cameraPos);

        bool mIsFirst;
        unsigned int mCurrVB;
        unsigned int mCurrTFB;
        std::vector<GLuint> mParticleBuffer;
        std::vector<GLuint> mTransformFeedback;
        PSUpdateTechnique mUpdateTechnique;
        BillboardTechnique mBillboardTechnique;
        RandomTexture mRandomTexture;
        std::unique_ptr<Texture> mTexture;
        int mTime;
};

#endif  // CG_LAB1_PARTICLESYSTEM_H
