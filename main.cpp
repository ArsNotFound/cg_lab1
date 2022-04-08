#include <GL/glew.h>
#include <GL/freeglut.h>
#include <glm/vec3.hpp>
#include <iostream>
#include <glm/ext/matrix_float4x4.hpp>

// Хранит указатель на буфер вершин
GLuint VBO;

// Указатель на всемирную матрицу
GLuint gWorldLocation;

// Программа-шейдер
static const std::string pVS = "// Версия шейдера 3.3                                                      \n"
                               "#version 330                                                               \n"
                               "// Входной тип данных - vec3 в позиции 0                                   \n"
                               "layout (location = 0) in vec3 Position;                                    \n"
                               "// uniform-переменная типа mat4                                            \n"
                               "uniform mat4 gWorld;                                                       \n"
                               "                                                                           \n"
                               "void main() {                                                              \n"
                               "  // Умножаем вектор вершин на всемирную матрицу для смещения треугольника \n"
                               "  gl_Position = gWorld * vec4(Position, 1.0);                              \n"
                               "}";

void renderSceneCallback() {
    // Очищаем окно
    glClear(GL_COLOR_BUFFER_BIT);

    // Переменная для изменения значения X
    static float Scale = -1.0f;

    // С каждой отрисовкой увеличиваем Scale
    Scale += 0.001f;

    // Подготавливаем матрицу 4x4 для изменения координаты X на значение синуса Scale
    glm::mat4 World;
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

    // Разрешаем использование каждого атрибута вершины в конвейере (аттрибут вершины)
    glEnableVertexAttribArray(0);
    // Обратно привязываем буфер, приготавливая для отрисовки
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    // Говорим конвейеру, как воспринимать данные внутри буфера (индекс атрибута, количество аттрибутов,
    // тип данных каждого компонента, нормализировать ли данные перед использованием, шаг - число байтов
    // между двумя экземплярами атрибута, смещение первого компонента первого универсального атрибута вершины)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

    // Загружаем данные в uniform-переменные шейдера (адрес переменной, количество матриц,
    // передаётся ли матрица по строкам, указатель на первый элемент матрицы)
    glUniformMatrix4fv(gWorldLocation, 1, GL_TRUE, &World[0][0]);

    // Устанавливаем цвет треугольника (RGBA)
    glColor4f(0.f, 0.f, 0.f, 0.f);

    // Вызываем функцию отрисовки (режим рисования, индекс первого элемента в буфере, количество элементов)
    glDrawArrays(GL_TRIANGLES, 0, 3);

    // Отключаем использование каждого атрибута вершины в конвейере [хороший тон]
    glDisableVertexAttribArray(0);

    // Меняем фоновый буфер и буфер кадра местами
    glutSwapBuffers();
}

void initializeGlutCallbacks() {
    // Задаём функции отрисовки
    glutDisplayFunc(renderSceneCallback);
    glutIdleFunc(renderSceneCallback);
}

void createVertexBuffer() {
    glm::vec3 vertices[3];

    vertices[0] = glm::vec3(-0.5f, -0.5f, 0.0f);
    vertices[1] = glm::vec3(0.8f, -1.0f, 0.0f);
    vertices[2] = glm::vec3(0.0f, 0.6f, 0.0f);

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
}

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

int main(int argc, char **argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowSize(1024, 768);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Tutorial 06");

    // Инициализируем callback-функции
    initializeGlutCallbacks();

    GLenum res = glewInit();
    if (res != GLEW_OK) {
        std::cerr << "Error glew init: '" << glewGetErrorString(res) << "'" << std::endl;
        return 1;
    }

    glClearColor(0.5f, 0.5f, 0.5f, 0.0f);

    createVertexBuffer();

    compileShaders();

    glutMainLoop();

    return 0;
}
