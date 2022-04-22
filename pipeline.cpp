#include "pipeline.h"

inline void initScaleTrans(glm::mat4 &m, const float x, const float y, const float z) {
    m[0][0] = x;
    m[0][1] = 0.0f;
    m[0][2] = 0.0f;
    m[0][3] = 0.0f;
    m[1][0] = 0.0f;
    m[1][1] = y;
    m[1][2] = 0.0f;
    m[1][3] = 0.0f;
    m[2][0] = 0.0f;
    m[2][1] = 0.0f;
    m[2][2] = z;
    m[2][3] = 0.0f;
    m[3][0] = 0.0f;
    m[3][1] = 0.0f;
    m[3][2] = 0.0f;
    m[3][3] = 1.0f;
}

inline void initRotateTrans(glm::mat4 &m, const float x, const float y, const float z) {
    glm::mat4 rx, ry, rz;

    const float rad_x = ToRadian(x);
    const float rad_y = ToRadian(y);
    const float rad_z = ToRadian(z);

    rx[0][0] = 1.0f;
    rx[0][1] = 0.0f;
    rx[0][2] = 0.0f;
    rx[0][3] = 0.0f;
    rx[1][0] = 0.0f;
    rx[1][1] = cosf(rad_x);
    rx[1][2] = -sinf(rad_x);
    rx[1][3] = 0.0f;
    rx[2][0] = 0.0f;
    rx[2][1] = sinf(rad_x);
    rx[2][2] = cosf(rad_x);
    rx[2][3] = 0.0f;
    rx[3][0] = 0.0f;
    rx[3][1] = 0.0f;
    rx[3][2] = 0.0f;
    rx[3][3] = 1.0f;

    ry[0][0] = cosf(rad_y);
    ry[0][1] = 0.0f;
    ry[0][2] = -sinf(rad_y);
    ry[0][3] = 0.0f;
    ry[1][0] = 0.0f;
    ry[1][1] = 1.0f;
    ry[1][2] = 0.0f;
    ry[1][3] = 0.0f;
    ry[2][0] = sinf(rad_y);
    ry[2][1] = 0.0f;
    ry[2][2] = cosf(rad_y);
    ry[2][3] = 0.0f;
    ry[3][0] = 0.0f;
    ry[3][1] = 0.0f;
    ry[3][2] = 0.0f;
    ry[3][3] = 1.0f;

    rz[0][0] = cosf(rad_z);
    rz[0][1] = -sinf(rad_z);
    rz[0][2] = 0.0f;
    rz[0][3] = 0.0f;
    rz[1][0] = sinf(rad_z);
    rz[1][1] = cosf(rad_z);
    rz[1][2] = 0.0f;
    rz[1][3] = 0.0f;
    rz[2][0] = 0.0f;
    rz[2][1] = 0.0f;
    rz[2][2] = 1.0f;
    rz[2][3] = 0.0f;
    rz[3][0] = 0.0f;
    rz[3][1] = 0.0f;
    rz[3][2] = 0.0f;
    rz[3][3] = 1.0f;

    m = rz * ry * rx;
}

inline void initTranslationTrans(glm::mat4 &m, const float x, const float y, const float z) {
    m[0][0] = 1.0f;
    m[0][1] = 0.0f;
    m[0][2] = 0.0f;
    m[0][3] = x;
    m[1][0] = 0.0f;
    m[1][1] = 1.0f;
    m[1][2] = 0.0f;
    m[1][3] = y;
    m[2][0] = 0.0f;
    m[2][1] = 0.0f;
    m[2][2] = 1.0f;
    m[2][3] = z;
    m[3][0] = 0.0f;
    m[3][1] = 0.0f;
    m[3][2] = 0.0f;
    m[3][3] = 1.0f;
}

inline void initPerspectiveProj(glm::mat4 &m, float FOV, float width, float height, float zNear, float zFar) {
    const float ar = width / height;
    const float zRange = zNear - zFar;
    const float tanHalfFOV = tanf(ToRadian(FOV / 2.0f));

    m[0][0] = 1.0f / (tanHalfFOV * ar);
    m[0][1] = 0.0f;
    m[0][2] = 0.0f;
    m[0][3] = 0.0;
    m[1][0] = 0.0f;
    m[1][1] = 1.0f / tanHalfFOV;
    m[1][2] = 0.0f;
    m[1][3] = 0.0;
    m[2][0] = 0.0f;
    m[2][1] = 0.0f;
    m[2][2] = (-zNear - zFar) / zRange;
    m[2][3] = 2.0f * zFar * zNear / zRange;
    m[3][0] = 0.0f;
    m[3][1] = 0.0f;
    m[3][2] = 1.0f;
    m[3][3] = 0.0;
}

inline void initCameraTrans(glm::mat4 &m, const glm::vec3 &target, const glm::vec3 &up) {
    glm::vec3 N = glm::normalize(target);
    glm::vec3 U = glm::normalize(up);
    U = glm::cross(U, target);
    glm::vec3 V = glm::cross(N, U);

    m[0][0] = U.x;
    m[0][1] = U.y;
    m[0][2] = U.z;
    m[0][3] = 0.0f;
    m[1][0] = V.x;
    m[1][1] = V.y;
    m[1][2] = V.z;
    m[1][3] = 0.0f;
    m[2][0] = N.x;
    m[2][1] = N.y;
    m[2][2] = N.z;
    m[2][3] = 0.0f;
    m[3][0] = 0.0f;
    m[3][1] = 0.0f;
    m[3][2] = 0.0f;
    m[3][3] = 1.0f;
}


const glm::mat4 *Pipeline::getTransformation() {
    // Матрицы преобразований
    glm::mat4 scaleTrans, rotateTrans, translationTrans, cameraTranslationTrans, cameraRotationTrans, persProjTrans;

    // Задаём необходимые параметры в матрицы
    initScaleTrans(scaleTrans, mScale.x, mScale.y, mScale.z);
    initRotateTrans(rotateTrans, mRotateInfo.x, mRotateInfo.y, mRotateInfo.z);
    initTranslationTrans(translationTrans, mWorldPos.x, mWorldPos.y, mWorldPos.z);
    initPerspectiveProj(persProjTrans, mPersProj.FOV, mPersProj.width, mPersProj.height, mPersProj.zNear,
                        mPersProj.zFar);
    initTranslationTrans(cameraTranslationTrans, -mCamera.pos.x, -mCamera.pos.y, -mCamera.pos.z);
    initCameraTrans(cameraRotationTrans, mCamera.target, mCamera.up);

    // Итоговая матрица получается в результате перемножения матриц преобразований в указанном порядке
    mTransformation =
            persProjTrans * cameraRotationTrans * cameraTranslationTrans * translationTrans * rotateTrans * scaleTrans;
    return &mTransformation;
}

