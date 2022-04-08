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
    GLchar errorLog[1024] = {0};

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