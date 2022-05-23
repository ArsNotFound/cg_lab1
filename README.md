# CG LAB 3

Третья лабораторная работа по компьютерной графике.

## Выполнено:

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

### Наложение текстур на фигуру
```glsl
in vec2 TexCoord0;
 
out vec4 FragColor;
 
uniform sampler2D gSampler;
 
void main()
{
    FragColor = texture2D(gSampler, TexCoord0.st);
};
```

### Рефакторинг кода на модули

### Различные виды освещения

```glsl
// Версия шейдера 3.3
#version 330

const int MAX_POINT_LIGHTS = 3;
const int MAX_SPOT_LIGHTS = 3;

in vec2 TexCoord0;
in vec3 WorldPos0;
in vec3 Normal0;

out vec4 FragColor;

// Основа для всех видов
struct BaseLight {
    vec3 Color;
    float AmbientIntensity;
    float DiffuseIntensity;
};

// Направленный свет
struct DirectionalLight {
    BaseLight Base;
    vec3 Direction;
};

// Параметры затухания
struct Attenuation {
    float Constant;
    float Linear;
    float Exp;
};

// Точечный свет
struct PointLight {
    BaseLight Base;
    vec3 Position;
    Attenuation Attenuation;
};

// Прожектор
struct SpotLight {
    PointLight Base;
    vec3 Direction;
    float Cutoff;
};

uniform DirectionalLight gDirectionalLight;

uniform int gNumPointLights;
uniform PointLight gPointLights[MAX_POINT_LIGHTS];

uniform int gNumSpotLights;
uniform SpotLight gSpotLights[MAX_SPOT_LIGHTS];

uniform sampler2D gSampler;

uniform vec3 gEyeWorldPos;
uniform float gMatSpecularIntensity;
uniform float gMatSpecularPower;

// Общая функция вычисления света
vec4 calcLightInternal(BaseLight Light, vec3 LightDirection, vec3 Normal) {
    vec4 AmbientColor = vec4(Light.Color, 1.0f) * Light.AmbientIntensity;
    float DiffuseFactor = dot(Normal, -LightDirection);

    vec4 DiffuseColor = vec4(0, 0, 0, 0);
    vec4 SpecularColor = vec4(0, 0, 0, 0);

    if (DiffuseFactor > 0) {
        DiffuseColor = vec4(Light.Color, 1.0f) *
        Light.DiffuseIntensity * DiffuseFactor;

        vec3 VertexToEye = normalize(gEyeWorldPos - WorldPos0);
        vec3 LightReflect = normalize(reflect(LightDirection, Normal));
        float SpecularFactor = dot(VertexToEye, LightReflect);
        SpecularFactor = pow(SpecularFactor, gMatSpecularPower);
        if (SpecularFactor > 0) {
            SpecularColor = vec4(Light.Color, 1.0f)
            * gMatSpecularIntensity * SpecularFactor;
        }
    }

    return (AmbientColor + DiffuseColor + SpecularColor);
}

// Функция вычисления направленного света
vec4 calcDirectionalLight(vec3 Normal) {
    return calcLightInternal(gDirectionalLight.Base, gDirectionalLight.Direction, Normal);
}

// Функция вычисления точечного света
vec4 calcPointLight(PointLight l, vec3 Normal) {
    vec3 LightDirection = WorldPos0 - l.Position;
    float Distance = length(LightDirection);
    LightDirection = normalize(LightDirection);

    vec4 Color = calcLightInternal(l.Base, LightDirection, Normal);
    float Attenuation = l.Attenuation.Constant +
                        l.Attenuation.Linear * Distance +
                        l.Attenuation.Exp * Distance * Distance;

    return Color / Attenuation;
}

// Функция вычисления прожектора
vec4 calcSpotLight(SpotLight l, vec3 Normal) {
    vec3 LightToPixel = normalize(WorldPos0 - l.Base.Position);
    float SpotFactor = dot(LightToPixel, l.Direction);

    if (SpotFactor > l.Cutoff) {
        vec4 Color = calcPointLight(l.Base, Normal);
        return Color * (1.0 - (1.0 - SpotFactor) * 1.0/(1.0 - l.Cutoff));
    } else {
        return vec4(0, 0, 0, 0);
    }
}

void main() {
    vec3 Normal = normalize(Normal0);
    vec4 TotalLight = calcDirectionalLight(Normal);

    for (int i = 0; i < gNumPointLights; i++) {
        TotalLight += calcPointLight(gPointLights[i], Normal);
    }

    for (int i = 0; i < gNumSpotLights; i++) {
        TotalLight += calcSpotLight(gSpotLights[i], Normal);
    }

    FragColor = texture2D(gSampler, TexCoord0.xy) * TotalLight;
}
```
