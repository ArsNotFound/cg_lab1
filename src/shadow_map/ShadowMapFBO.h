#ifndef CG_LAB1_SHADOWMAPFBO_H
#define CG_LAB1_SHADOWMAPFBO_H

#include <GL/glew.h>

class ShadowMapFBO {
    public:
        ShadowMapFBO();

        ~ShadowMapFBO();

        bool init(unsigned int windowWidth, unsigned int windowHeight);

        void bindForWriting() const;

        void bindForReading(GLenum textureUnit) const;

    private:
        GLuint mFBO;
        GLuint mShadowMap;
};

#endif  // CG_LAB1_SHADOWMAPFBO_H
