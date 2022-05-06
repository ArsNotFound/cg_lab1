#include "pipeline.h"
#include <glm/trigonometric.hpp>

// Создание матрицы масштаба
inline glm::mat4 initScaleTrans(const float x, const float y, const float z) {
    glm::mat4 m = glm::mat4(1.0f);
    m[0][0] = x;
    m[1][1] = y;
    m[2][2] = z;
    return m;
}

// Создание матрицы вращения
inline glm::mat4 initRotateTrans(const float x, const float y, const float z) {
    glm::mat4 rx(1.0f), ry(1.0f), rz(1.0f);

    const float rad_x = glm::radians(x);
    const float rad_y = glm::radians(y);
    const float rad_z = glm::radians(z);

    rx[1][1] = cosf(rad_x);
    rx[1][2] = -sinf(rad_x);
    rx[2][1] = sinf(rad_x);
    rx[2][2] = cosf(rad_x);

    ry[0][0] = cosf(rad_y);
    ry[0][2] = -sinf(rad_y);
    ry[2][0] = sinf(rad_y);
    ry[2][2] = cosf(rad_y);

    rz[0][0] = cosf(rad_z);
    rz[0][1] = -sinf(rad_z);
    rz[1][0] = sinf(rad_z);
    rz[1][1] = cosf(rad_z);

    return rz * ry * rx;
}

// Создание матрицы перемещения
inline glm::mat4 initTranslationTrans(const float x, const float y, const float z) {
    glm::mat4 m = glm::mat4(1.0f);
    m[0][3] = x;
    m[1][3] = y;
    m[2][3] = z;
    return m;
}

// Создание матрицы перспективы
inline glm::mat4 initPerspectiveProj(float FOV, float width, float height, float zNear, float zFar) {
    float ar = width / height;
    float tanHalfFOV = tanf(glm::radians(FOV / 2.0f));

    glm::mat4 m = glm::mat4(0.0f);
    m[0][0] = 1.0f / (tanHalfFOV * ar);
    m[1][1] = 1.0f / tanHalfFOV;
    m[2][2] = (zNear + zFar) / (zFar - zNear);
    m[2][3] = 1.0f;
    m[3][2] = -2.0f * zFar * zNear / (zFar - zNear);

    return m;
}

// Создание матрицы камеры
inline glm::mat4 initCameraTrans(const glm::vec3 &target, const glm::vec3 &up) {
    glm::vec3 N = glm::normalize(target);
    glm::vec3 U = glm::normalize(up);
    U = glm::cross(U, target);
    glm::vec3 V = glm::cross(N, U);

    glm::mat4 m = glm::mat4(
            glm::vec4(U, 0.0f),
            glm::vec4(V, 0.0f),
            glm::vec4(N, 0.0f),
            glm::vec4(0.0f, 0.0f, 0.0f, 1.0f)
    );

    return m;
}

// Создание мировой матрицы
const glm::mat4 *Pipeline::getTransformation() {
    // Матрицы преобразований
    glm::mat4 scaleTrans = initScaleTrans(mScale.x, mScale.y, mScale.z);
    glm::mat4 rotateTrans = initRotateTrans(mRotateInfo.x, mRotateInfo.y, mRotateInfo.z);
    glm::mat4 translationTrans = initTranslationTrans(mWorldPos.x, mWorldPos.y, mWorldPos.z);
    glm::mat4 persProjTrans = initPerspectiveProj(
            mPersProj.FOV,
            mPersProj.width,
            mPersProj.height,
            mPersProj.zNear,
            mPersProj.zFar
    );

    // Преобразования камеры
    glm::mat4 cameraTranslationTrans = initTranslationTrans(-mCamera.pos.x, -mCamera.pos.y, -mCamera.pos.z);
    glm::mat4 cameraRotationTrans = initCameraTrans(mCamera.target, mCamera.up);

    // Итоговая матрица получается в результате перемножения матриц преобразований в указанном порядке
    mTransformation =
            persProjTrans *
            cameraRotationTrans *
            cameraTranslationTrans *
            translationTrans *
            rotateTrans *
            scaleTrans;
    return &mTransformation;
}
