#include "Camera.h"

#include <GL/freeglut.h>

#include <iostream>

#include <glm/geometric.hpp>
#include <glm/gtx/string_cast.hpp>
#include <glm/gtx/rotate_vector.hpp>

// Конструкторы камеры
Camera::Camera(int windowWidth, int windowHeight)
    : mWindowWidth(windowWidth),
      mWindowHeight(windowHeight),
      mPos(glm::vec3(0.0f, 0.0f, 0.0f)),
      mTarget(glm::normalize(glm::vec3(0.0f, 0.0f, 1.0f))),
      mUp(glm::vec3(0.0f, 1.0f, 0.0f)) {
    init();
}

Camera::Camera(
    int windowWidth,
    int windowHeight,
    const glm::vec3 &pos,
    const glm::vec3 &target,
    const glm::vec3 &up
)
    : mWindowWidth(windowWidth),
      mWindowHeight(windowHeight),
      mPos(pos),
      mTarget(glm::normalize(target)),
      mUp(glm::normalize(up)) {
    init();
}

// Инициализация камеры
void Camera::init() {
    glm::vec3 hTarget = glm::normalize(glm::vec3(mTarget.x, 0, mTarget.z));

    if (hTarget.z >= 0) {
        if (hTarget.x >= 0)
            mAngleH = 360.0f - glm::degrees(asinf(hTarget.z));
        else
            mAngleH = 180.0f + glm::degrees(asinf(hTarget.z));
    } else {
        if (hTarget.x >= 0)
            mAngleH = glm::degrees(asinf(-hTarget.z));
        else
            mAngleH = 90.0f + glm::degrees(asinf(-hTarget.z));
    }

    mAngleV = -glm::degrees(asinf(mTarget.y));

    mOnUpperEdge = false;
    mOnLowerEdge = false;
    mOnLeftEdge = false;
    mOnRightEdge = false;
    mMousePos.x = mWindowWidth / 2;
    mMousePos.y = mWindowHeight / 2;

    glutWarpPointer(mMousePos.x, mMousePos.y);
}

const float stepSize = 0.4f;

// Функция обработки нажатия на клавишу движения
bool Camera::onKeyPressed(int key) {
    switch (key) {
        case GLUT_KEY_UP: {
            mPos += mTarget * stepSize;
            return true;
        }
        case GLUT_KEY_DOWN: {
            mPos -= mTarget * stepSize;
            return true;
        }
        case GLUT_KEY_LEFT: {
            auto left = glm::normalize(glm::cross(mTarget, mUp)) * stepSize;
            mPos += left;
            return true;
        }
        case GLUT_KEY_RIGHT: {
            auto right = glm::normalize(glm::cross(mUp, mTarget)) * stepSize;
            mPos += right;
            return true;
        }
        default:
            return false;
    }
}

const int MARGIN = 10;

// Функция обработки движения мыши
void Camera::onMouse(int x, int y) {
    const int dX = x - mMousePos.x;
    const int dY = y - mMousePos.y;

    mMousePos.x = x;
    mMousePos.y = y;

    mAngleH += (float)dX / 20.0f;
    mAngleV += (float)dY / 20.0f;

    if (dX == 0) {
        if (x <= MARGIN)
            mOnLeftEdge = true;
        else if (x >= (mWindowWidth - MARGIN))
            mOnRightEdge = true;
    } else {
        mOnLeftEdge = false;
        mOnRightEdge = false;
    }

    if (dY == 0) {
        if (y <= MARGIN)
            mOnUpperEdge = true;
        else if (y >= (mWindowHeight - MARGIN))
            mOnLowerEdge = true;
    } else {
        mOnUpperEdge = false;
        mOnLowerEdge = false;
    }

    update();
}

// Функция обновления параметров камеры
void Camera::update() {
    const glm::vec3 vAxis(0.0f, 1.0f, 0.0f);

    glm::vec3 view(1.0f, 0.0f, 0.0f);
    view = glm::rotate(view, glm::radians(mAngleH), vAxis);
    view = glm::normalize(view);

    glm::vec3 hAxis = glm::cross(vAxis, view);
    hAxis = glm::normalize(hAxis);
    view = glm::rotate(view, glm::radians(mAngleV), hAxis);

    mTarget = glm::normalize(view);
    mUp = glm::normalize(glm::cross(mTarget, hAxis));
}

// Функция обновления параметров камеры при рендере
void Camera::onRender() {
    bool shouldUpdate = false;

    if (mOnLeftEdge) {
        mAngleH -= 0.1f;
        shouldUpdate = true;
    } else if (mOnRightEdge) {
        mAngleH += 0.1f;
        shouldUpdate = true;
    }

    if (mOnUpperEdge && mAngleV > -90.0f) {
        mAngleV -= 0.1f;
        shouldUpdate = true;
    } else if (mOnLowerEdge && mAngleV < 90.0f) {
        mAngleV += 0.1f;
        shouldUpdate = true;
    }

    if (shouldUpdate) update();
}
