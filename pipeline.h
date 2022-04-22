#ifndef CG_LAB1_PIPELINE_H
#define CG_LAB1_PIPELINE_H

#include <cmath>
#include <glm/detail/type_mat4x4.hpp>

// Макросы преобразования градусов в радианы и наоборот
#define ToRadian(x) ((x) * M_PI / 180.0f)

// Пайплайн для преобразований
class Pipeline {
public:
    // Конструктор для начальных значений
    Pipeline() : mScale(glm::vec3(1.0f, 1.0f, 1.0f)),
                 mWorldPos(glm::vec3(0.0f, 0.0f, 0.0f)),
                 mRotateInfo(glm::vec3(0.0f, 0.0f, 0.0f)),
                 mPersProj(),
                 mTransformation(glm::mat4()) {}

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

    // Функция задания вращения
    void setRotation(float rotateX, float rotateY, float rotateZ) {
        mRotateInfo[0] = rotateX;
        mRotateInfo[1] = rotateY;
        mRotateInfo[2] = rotateZ;
    }

    void setPerspectiveProj(float FOV, float width, float height, float zNear, float zFar) {
        mPersProj.FOV = FOV;
        mPersProj.width = width;
        mPersProj.height = height;
        mPersProj.zNear = zNear;
        mPersProj.zFar = zFar;
    }

    void setCamera(const glm::vec3& pos, const glm::vec3 &target, const glm::vec3 &up) {
        mCamera.pos = pos;
        mCamera.target = target;
        mCamera.up = up;
    }

    // Функция получения итоговой матрицы
    const glm::mat4 *getTransformation();

private:
    // Необходимые переменные
    glm::vec3 mScale;
    glm::vec3 mWorldPos;
    glm::vec3 mRotateInfo;

    struct {
        float FOV;
        float width;
        float height;
        float zNear;
        float zFar;
    } mPersProj;

    struct {
        glm::vec3 pos;
        glm::vec3 target;
        glm::vec3 up;
    } mCamera;

    glm::mat4 mTransformation;
};


#endif //CG_LAB1_PIPELINE_H
