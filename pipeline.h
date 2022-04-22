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
    void scale(float scaleX, float scaleY, float scaleZ) {
        mScale[0] = scaleX;
        mScale[1] = scaleY;
        mScale[2] = scaleZ;
    }

    // Функция задания изменения положения
    void worldPos(float x, float y, float z) {
        mWorldPos[0] = x;
        mWorldPos[1] = y;
        mWorldPos[2] = z;
    }

    // Функция задания вращения
    void rotate(float rotateX, float rotateY, float rotateZ) {
        mRotateInfo[0] = rotateX;
        mRotateInfo[1] = rotateY;
        mRotateInfo[2] = rotateZ;
    }

    void perspectiveProj(float FOV, float width, float height, float zNear, float zFar) {
        mPersProj.FOV = FOV;
        mPersProj.width = width;
        mPersProj.height = height;
        mPersProj.zNear = zNear;
        mPersProj.zFar = zFar;
    }

    // Функция получения итоговой матрицы
    const glm::mat4 *getTransformation();

private:
    // Вспомогательные функции
    void initScaleTrans(glm::mat4 &m) const;
    void initRotateTrans(glm::mat4 &m) const;
    void initTranslationTrans(glm::mat4 &m) const;
    void initPerspectiveProj(glm::mat4 &m) const;

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

    glm::mat4 mTransformation;
};


#endif //CG_LAB1_PIPELINE_H
