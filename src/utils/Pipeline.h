#ifndef CG_LAB1_PIPELINE_H
#define CG_LAB1_PIPELINE_H

#include <cmath>

#include <glm/glm.hpp>

struct PersProjInfo {
        float FOV;     // Поле зрения (в градусах)
        float width;   // Ширина экрана
        float height;  // Высота экрана
        float zNear;   // Ближайшая глубина зрения
        float zFar;    // Дальнейшая глубина зрения
};

// Пайплайн для преобразований
class Pipeline {
    public:
        // Конструктор для начальных значений
        Pipeline()
            : mScale(glm::vec3(1.0f, 1.0f, 1.0f)),
              mWorldPos(glm::vec3(0.0f, 0.0f, 0.0f)),
              mRotation(glm::vec3(0.0f, 0.0f, 0.0f)),
              mPersProj(),
              mWVPTransformation(glm::mat4()) {}

        // Функция задания изменения масштаба
        void setScale(float scaleX, float scaleY, float scaleZ) {
            mScale[0] = scaleX;
            mScale[1] = scaleY;
            mScale[2] = scaleZ;
        }

        // Функция задания изменения положения
        void setWorldPos(float x, float y, float z) {
            mWorldPos[0] = x;
            mWorldPos[1] = y;
            mWorldPos[2] = z;
        }

        void setWorldPos(const glm::vec3 &vec) { mWorldPos = vec; }

        // Функция задания вращения
        void setRotation(float rotateX, float rotateY, float rotateZ) {
            mRotation[0] = rotateX;
            mRotation[1] = rotateY;
            mRotation[2] = rotateZ;
        }

        void setPerspectiveProj(
            float FOV, float width, float height, float zNear, float zFar
        ) {
            mPersProj.FOV = FOV;
            mPersProj.width = width;
            mPersProj.height = height;
            mPersProj.zNear = zNear;
            mPersProj.zFar = zFar;
        }

        void setPerspectiveProj(PersProjInfo p) { mPersProj = p; }

        void setCamera(
            const glm::vec3 &pos, const glm::vec3 &target, const glm::vec3 &up
        ) {
            mCamera.pos = pos;
            mCamera.target = target;
            mCamera.up = up;
        }

        const glm::mat4 &getWorldTransformation();

        const glm::mat4 &getWVPTransformation();

    private:
        glm::vec3 mScale;     // Масштаб
        glm::vec3 mWorldPos;  // Позиция
        glm::vec3 mRotation;  // Вращение

        PersProjInfo mPersProj;

        struct {
                glm::vec3 pos;     // Позиция камеры
                glm::vec3 target;  // Направление камеры
                glm::vec3 up;      // "Вверх" камеры
        } mCamera;

        glm::mat4 mWVPTransformation;
        glm::mat4 mWorldTransformation;
};

#endif  // CG_LAB1_PIPELINE_H
