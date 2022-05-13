# CG LAB 3

Третья лабораторная работа по компьютерной графике.

## Выполнено:

### Рефакторинг класса Pipeline

### Создание класса камеры

```c++
class Camera {
public:
    Camera(int windowWidth, int windowHeight);

    Camera(int windowWidth, int windowHeight, const glm::vec3 &pos, const glm::vec3 &target, const glm::vec3 &up);

    const glm::vec3 &getPos() const {
        return mPos;
    }

    const glm::vec3 &getTarget() const {
        return mTarget;
    }

    const glm::vec3 &getUp() const {
        return mUp;
    }

    bool onKeyPressed(int key);

    void onMouse(int x, int y);

    void onRender();

private:
    void init();

    void update();

    glm::vec3 mPos;    // Позиция камеры
    glm::vec3 mTarget; // Направление камеры
    glm::vec3 mUp;     // "Вверх" камеры

    int mWindowWidth;  // Ширина экрана
    int mWindowHeight; // Высота экрана

    float mAngleH;     // Горизонтальный угол
    float mAngleV;     // Вертикальный угол

    // Переменные, показывающие, что камера находится на одном из краёв экрана
    bool mOnUpperEdge;
    bool mOnLowerEdge;
    bool mOnLeftEdge;
    bool mOnRightEdge;

    glm::ivec2 mMousePos; // Позиция мыши
};
```

### Задание callback функций для обработки нажатий на клавиши и движения мыши

```c++
// Callback нажатия клавиши
void specialKeyboardCallback(int key, __attribute__((unused)) int x, __attribute__((unused)) int y) {
gameCamera->onKeyPressed(key);
}

// Callback движения мыши
void passiveMouseCallback(int x, int y) {
gameCamera->onMouse(x, y);
}

// Callback нажатия клавиши
void keyboardCallback(unsigned char Key, int x, int y) {
switch (Key) {
case 'q':
exit(0);
}
}
```

### Создание класса Texture

```c++
class Texture {
public:
    Texture(GLenum textureTarget, const std::string& fileName);
    // Функция загрузки текстуры
    bool load();
    // Функция привязки текстуры
    void bind(GLenum textureUnit);

private:
    std::string mFileName;  // Имя файла
    GLenum mTextureTarget;  // Куда ставится текстура
    GLuint mTextureObj;     // Объект текстуры
    Magick::Image* mPImage; // Изображение
    Magick::Blob mBlob;     // Данные изображения
};

```