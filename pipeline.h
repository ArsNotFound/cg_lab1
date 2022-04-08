#ifndef CG_LAB1_PIPELINE_H
#define CG_LAB1_PIPELINE_H

#include <cmath>
#include <glm/detail/type_mat4x4.hpp>

// Макросы преобразования градусов в радианы и наоборот
#define ToRadian(x) ((x) * M_PI / 180.0f)
#define ToDegree(x) ((x) * 180.0f / M_PI)

// Пайплайн для преобразований
class Pipeline {
public:
    // Конструктор для начальных значений
    Pipeline() : mScale(glm::vec3(1.0f, 1.0f, 1.0f)),
                 mWorldPos(glm::vec3(0.0f, 0.0f, 0.0f)),
                 mRotateInfo(glm::vec3(0.0f, 0.0f, 0.0f)),
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

    // Функция получения итоговой матрицы
    const glm::mat4 *getTransformation();

private:
    // Вспомогательные функции
    void initScaleTransform(glm::mat4 &m) const;
    void initRotateTransform(glm::mat4 &m) const;
    void initTranslationTransform(glm::mat4 &m) const;

    // Необходимые переменные
    glm::vec3 mScale;
    glm::vec3 mWorldPos;
    glm::vec3 mRotateInfo;

    glm::mat4 mTransformation;
};


#endif //CG_LAB1_PIPELINE_H
