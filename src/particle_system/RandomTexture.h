#ifndef CG_LAB1_RANDOMTEXTURE_H
#define CG_LAB1_RANDOMTEXTURE_H

#include <GL/glew.h>

class RandomTexture {
    public:
        RandomTexture();

        ~RandomTexture();

        bool init(unsigned int size);

        void bind(GLenum textureUnit) const;

    private:
        GLuint mTextureObj;
};

#endif  // CG_LAB1_RANDOMTEXTURE_H
