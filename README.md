# CG LAB 2

Вторая лабораторная работа по компьютерной графике.

## Выполнено:

### Перемещение треугольника по координате X:

Создаём глобальную переменную gWorldLocation:

```c++
GLuint gWorldLocation;
```

Добавляем код шейдера:

```glsl
// Версия шейдера 3.3                                                      
#version 330                                                               
// Входной тип данных - vec3 в позиции 0                                   
layout (location = 0) in vec3 Position;
// uniform-переменная типа mat4                                            
uniform mat4 gWorld;

void main() {
    // Умножаем вектор вершин на всемирную матрицу для смещения треугольника
    gl_Position = gWorld * vec4(Position, 1.0);
}
```

В функцию отрисовки добавляем:

```c++
// Переменная для изменения значения X
static float Scale = -1.0f;

// С каждой отрисовкой увеличиваем Scale
Scale += 0.001f;

// Подготавливаем матрицу 4x4 для изменения координаты X на значение синуса Scale
glm::mat4 World;
World[0][0] = 1.0f;
World[0][1] = 0.0f;
World[0][2] = 0.0f;
World[0][3] = sinf(Scale);
World[1][0] = 0.0f;
World[1][1] = 1.0f;
World[1][2] = 0.0f;
World[1][3] = 0.0f;
World[2][0] = 0.0f;
World[2][1] = 0.0f;
World[2][2] = 1.0f;
World[2][3] = 0.0f;
World[3][0] = 0.0f;
World[3][1] = 0.0f;
World[3][2] = 0.0f;
World[3][3] = 1.0f;

// Загружаем данные в uniform-переменные шейдера (адрес переменной, количество матриц,
// передаётся ли матрица по строкам, указатель на первый элемент матрицы)
glUniformMatrix4fv(gWorldLocation, 1, GL_TRUE, &World[0][0]);
```

Добавляем две новые функции:

```c++
// Функция, добавляющая шейдер к программе
void addShader(GLuint shaderProgram, const std::string pShaderText, GLenum shaderType) {
// Создаём шейдер
GLuint shaderObj = glCreateShader(shaderType);

if (shaderObj == 0) {
std::cerr << "Error: creating shader type " << shaderType << std::endl;
exit(1);
}

// Сохраняем код шейдера в массиве
const GLchar *p[1];
p[0] = pShaderText.c_str();

// Массив длин кодов шейдеров
GLint lengths[1];
lengths[0] = pShaderText.length();

// Задаём исходники шейдера
glShaderSource(shaderObj, 1, p, lengths);
// Компилируем шейдер
glCompileShader(shaderObj);

// Проверяем, что шейдер успешно скомпилировался
GLint success;
glGetShaderiv(shaderObj, GL_COMPILE_STATUS, &success);
if (!success) {
GLchar infoLog[1024];
glGetShaderInfoLog(shaderObj, sizeof(infoLog), nullptr, infoLog);
std::cerr << "Error compiling shader type " << shaderType << ": '" << infoLog << "'" << std::endl;
exit(1);
}

// Добавляем шейдер в программу
glAttachShader(shaderProgram, shaderObj);
}

// Функция, компилирующая программу-шейдер
void compileShaders() {
// Создаём программу-шейдер
GLuint shaderProgram = glCreateProgram();
if (shaderProgram == 0) {
std::cerr << "Error creating shader program" << std::endl;
exit(1);
}

// Добавляем шейдер для вершин
addShader(shaderProgram, pVS, GL_VERTEX_SHADER);

GLint success = 0;
GLchar errorLog[1024] = { 0 };

// Линкуем программу
glLinkProgram(shaderProgram);
// Проверяем, что линковка прошла успешно
glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
if (success == 0) {
glGetProgramInfoLog(shaderProgram, sizeof(errorLog), nullptr, errorLog);
std::cerr << "Error linking shader program: '" << errorLog << "'" << std::endl;
exit(1);
}

// Валидируем программу
glValidateProgram(shaderProgram);
// Проверяем, что валидация прошла успешно
glGetProgramiv(shaderProgram, GL_VALIDATE_STATUS, &success);
if (success == 0) {
glGetProgramInfoLog(shaderProgram, sizeof(errorLog), nullptr, errorLog);
std::cerr << "Invalid shader program: '" << errorLog << "'" << std::endl;
exit(1);
}

// Указываем OpenGL, что надо использовать эту программу
glUseProgram(shaderProgram);

// Сохраняем местоположение переменной gWorld
gWorldLocation = glGetUniformLocation(shaderProgram, "gWorld");
assert(gWorldLocation != 0xFFFFFFFF);
}
```

### Вращение треугольника вокруг оси OZ:

В функции отрисовки меняем матрицу World:

```c++
World[0][0] = cosf(Scale);
World[0][1] = -sinf(Scale);
World[0][2] = 0.0f;
World[0][3] = 0.0f;
World[1][0] = sinf(Scale);
World[1][1] = cosf(Scale);
World[1][2] = 0.0f;
World[1][3] = 0.0f;
World[2][0] = 0.0f;
World[2][1] = 0.0f;
World[2][2] = 1.0f;
World[2][3] = 0.0f;
World[3][0] = 0.0f;
World[3][1] = 0.0f;
World[3][2] = 0.0f;
World[3][3] = 1.0f;
```

### Преобразования масштаба:

По аналогии с прошлым пунктом меняем матрицу World:

```c++
World[0][0] = sinf(Scale);
World[0][1] = 0.0f;
World[0][2] = 0.0f;
World[0][3] = 0.0f;
World[1][0] = 0.0f;
World[1][1] = cosf(Scale);
World[1][2] = 0.0f;
World[1][3] = 0.0f;
World[2][0] = 0.0f;
World[2][1] = 0.0f;
World[2][2] = sinf(Scale);
World[2][3] = 0.0f;
World[3][0] = 0.0f;
World[3][1] = 0.0f;
World[3][2] = 0.0f;
World[3][3] = 1.0f;
```

### Объединение преобразований:

Создадим класс с пайплайном преобразований (приведён header-файл, реализацию смотреть в
файле [pipeline.cpp](pipeline.cpp)):

```c++
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
    void initScaleTrans(glm::mat4 &m) const;
    void initRotateTrans(glm::mat4 &m) const;
    void initTranslationTrans(glm::mat4 &m) const;

    // Необходимые переменные
    glm::vec3 mScale;
    glm::vec3 mWorldPos;
    glm::vec3 mRotateInfo;

    glm::mat4 mTransformation;
};
```

В функции отрисовки вместо ручного задания матрицы World используем пайплайн:

```c++
// Создаём pipeline для трансформаций
Pipeline p;
// Меняем масштаб
p.scale(sinf(Scale * 0.1f), sinf(Scale * 0.1f), sinf(Scale * 0.1f));
// Двигаем треугольник по оси X
p.worldPos(sinf(Scale), 0.0f, 0.0f);
// Вращаем его по всем осям
p.rotate(sinf(Scale) * 90.0f, sinf(Scale) * 90.0f, sinf(Scale) * 90.0f);

// Загружаем данные в uniform-переменные шейдера (адрес переменной, количество матриц,
// передаётся ли матрица по строкам, указатель на первый элемент матрицы)
glUniformMatrix4fv(gWorldLocation, 1, GL_TRUE, (const GLfloat *) p.getTransformation());
```

### Проекция перспективы

В [main.cpp](main.cpp) зададим параметры окна:
```c++
// Задаём параметры окна
#define WINDOW_WIDTH 1024
#define WINDOW_HEIGHT 768
```

В пайплайн добавляем вспомогательные методы и переменную:

```c++
void initPerspectiveProj(glm::mat4 &m) const;

struct {
    float FOV
    float width;
    float height;
    float zNear;
    float zFar;
} mPersProj;

void perspectiveProj(float FOV, float width, float height, float zNear, float zFar) {
    mPersProj.FOV = FOV;
    mPersProj.width = width;
    mPersProj.height = height;
    mPersProj.zNear = zNear;
    mPersProj.zFar = zFar;
}
```

В методе getTransformation добавляем в умножение матрицу перспективы:

```c++
mTransformation = persProjTrans * translationTrans * rotateTrans * scaleTrans;
```

В шейдер вершин добавим вычисление цвета:

```glsl
Color = vec4(clamp(Position, 0.0, 0.5), 1.0);  
```

Создадим шейдер фрагментов для установки цвета:

```glsl
// Версия шейдера 3.3                                                      
#version 330                                                               

in vec4 Color;
out vec4 FragColor;

void main() {
    // Устанавливаем цвет                                                    
    FragColor = Color;
} 
```

Изменим преобразования матрицы:
```c++
// Создаём pipeline для трансформаций
Pipeline p;
// Меняем масштаб
p.scale(0.1f, 0.1f, 0.1f);
// Вращаем фигуру
p.rotate(0, Scale, 0);
// Устанавливаем положение фигуры
p.worldPos(0.0f, 0.0f, 100.0f);
// Задаём проекцию перспективы
p.perspectiveProj(90.0f, WINDOW_WIDTH, WINDOW_HEIGHT, 10.0f, 10000.0f);
```

Создадим вектор индексов:
```c++
// Хранит указатель на буфер индексов
GLuint IBO;

void createIndexBuffer() {
    unsigned int indices[] = {
        0, 3, 1,
        1, 3, 2,
        2, 3, 0,
        0, 2, 1
    };

    glGenBuffers(1, &IBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
}
```

В функции отрисовки скажем OpenGL, что мы рисуем несколько треугольников:
```c++
glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, nullptr);
```
