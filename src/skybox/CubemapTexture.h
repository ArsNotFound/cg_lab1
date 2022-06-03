#ifndef CG_LAB1_CUBEMAPTEXTURE_H
#define CG_LAB1_CUBEMAPTEXTURE_H

#include <GL/glew.h>

#include <string>
#include <vector>

#include <sail-c++/sail-c++.h>

class CubemapTexture {
    public:
        CubemapTexture(
            const std::string& directory,
            const std::string& poxXFilename,
            const std::string& negXFilename,
            const std::string& posYFilename,
            const std::string& negYFilename,
            const std::string& posZFilename,
            const std::string& negZFilename
        );

        ~CubemapTexture();

        bool load();

        void bind(GLenum textureUnit) const;

    private:
        std::vector<std::string> mFilenames;
        GLuint mTextureObj;
};

#endif  // CG_LAB1_CUBEMAPTEXTURE_H
