#include "Pipeline.h"

#include <glm/gtx/transform.hpp>
#include <glm/trigonometric.hpp>

const glm::mat4 &Pipeline::getWorldTransformation() {
    static const glm::vec3 xAxis(1.0f, 0.0f, 0.0f);
    static const glm::vec3 yAxis(0.0f, 1.0f, 0.0f);
    static const glm::vec3 zAxis(0.0f, 0.0f, 1.0f);

    glm::mat4 trans(1.0f);
    trans = glm::translate(trans, mWorldPos);
    trans = glm::rotate(trans, glm::radians(mRotation.x), xAxis);
    trans = glm::rotate(trans, glm::radians(mRotation.y), yAxis);
    trans = glm::rotate(trans, glm::radians(mRotation.z), zAxis);
    trans = glm::scale(trans, mScale);

    mWorldTransformation = trans;
    return mWorldTransformation;
}

const glm::mat4 &Pipeline::getWVPTransformation() {
    glm::mat4 persProjTrans = glm::perspectiveFov(
        glm::radians(mPersProj.FOV),
        mPersProj.width,
        mPersProj.height,
        mPersProj.zNear,
        mPersProj.zFar
    );

    glm::mat4 cameraTrans = glm::lookAt(mCamera.pos, mCamera.target, mCamera.up);

    mWVPTransformation = persProjTrans * cameraTrans * getWorldTransformation();
    return mWVPTransformation;
}
