#include "GLUTBackend.h"

#include <GL/glew.h>

#include <GL/freeglut.h>

#include <iostream>

#include <boost/format.hpp>

std::shared_ptr<ICallbacks> sCallbacks;

void specialKeyboardCB(int key, int x, int y) {
    sCallbacks->specialKeyboardCB(key, x, y);
}

void keyboardCB(unsigned char key, int x, int y) {
    sCallbacks->keyboardCB(key, x, y);
}

void passiveMouseCB(int x, int y) { sCallbacks->passiveMouseCB(x, y); }

void renderSceneCB() { sCallbacks->renderSceneCB(); }

void idleCB() { sCallbacks->idleCB(); }

void initCallbacks() {
    glutDisplayFunc(renderSceneCB);
    glutIdleFunc(idleCB);
    glutSpecialFunc(specialKeyboardCB);
    glutPassiveMotionFunc(passiveMouseCB);
    glutKeyboardFunc(keyboardCB);
}

void GLUTBackend::init(int argc, char **argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
    glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE,
                  GLUT_ACTION_GLUTMAINLOOP_RETURNS);
}

bool GLUTBackend::createWindow(unsigned int width, unsigned int height,
                               bool isFullscreen, const std::string &title) {
    if (isFullscreen) {
        std::string modeString =
            (boost::format("%1%x%2%") % width % height).str();
        glutGameModeString(modeString.c_str());
        glutEnterGameMode();
    } else {
        glutInitWindowSize(static_cast<int>(width), static_cast<int>(height));
        glutCreateWindow(title.c_str());
    }

    GLenum res = glewInit();
    if (res != GLEW_OK) {
        std::cerr << "Error glew init: '" << glewGetErrorString(res) << "'"
                  << std::endl;
        return false;
    }

    return true;
}

void GLUTBackend::run(const std::shared_ptr<ICallbacks> &callbacks) {
    if (!callbacks) {
        std::cerr << __FUNCTION__ << " : callbacks not specified!" << std::endl;
        return;
    }

    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glFrontFace(GL_CW);
    glCullFace(GL_BACK);
    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);

    sCallbacks = callbacks;
    initCallbacks();
    glutMainLoop();
}
