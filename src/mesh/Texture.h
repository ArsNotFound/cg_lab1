#ifndef CG_LAB1_TEXTURE_H
#define CG_LAB1_TEXTURE_H

#include <GL/glew.h>

#include <string>

#include <sail-c++/sail-c++.h>

class Texture {
    public:
        Texture(GLenum textureTarget, std::string fileName);

        // Функция загрузки текстуры
        bool load();
        // Функция привязки текстуры
        void bind(GLenum textureUnit) const;

    private:
        std::string mFileName;  // Имя файла
        GLenum mTextureTarget;  // Куда ставится текстура
        GLuint mTextureObj;     // Объект текстуры
        sail::image mImage;     // Изображение
};

#endif  // CG_LAB1_TEXTURE_H
