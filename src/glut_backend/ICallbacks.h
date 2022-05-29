#ifndef CG_LAB1_ICALLBACKS_H
#define CG_LAB1_ICALLBACKS_H

class ICallbacks {
    public:
    virtual void specialKeyboardCB(int key, int x, int y) = 0;
    virtual void keyboardCB(unsigned char key, int x, int y) = 0;
    virtual void passiveMouseCB(int x, int y) = 0;
    virtual void renderSceneCB() = 0;
    virtual void idleCB() = 0;
    virtual ~ICallbacks() {};
};

#endif  // CG_LAB1_ICALLBACKS_H
