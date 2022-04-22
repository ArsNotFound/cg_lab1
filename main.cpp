#include <GL/glew.h>
#include <GL/freeglut.h>
#include <glm/vec3.hpp>
#include <iostream>
#include <glm/ext/matrix_float4x4.hpp>
#include <fstream>
#include "pipeline.h"

// Задаём параметры окна
#define WINDOW_WIDTH 1024
#define WINDOW_HEIGHT 768

// Хранит указатель на буфер вершин
GLuint VBO;

// Хранит указатель на буфер индексов
GLuint IBO;

// Указатель на всемирную матрицу
GLint gWorldLocation;

void renderSceneCallback() {
    glClear(GL_COLOR_BUFFER_BIT);

    // Переменная для изменения значений
    static float Scale = 0.0f;

    // С каждой отрисовкой увеличиваем Scale
    Scale += 0.1f;

    // Создаём pipeline для трансформаций
    Pipeline p;
    p.rotate(0.0f, Scale, 0.0f);
    p.worldPos(0.0f, 0.0f, 5.0f);
    p.perspectiveProj(60.0f, WINDOW_WIDTH, WINDOW_HEIGHT, 1.0f, 100.0f);

    glUniformMatrix4fv(gWorldLocation, 1, GL_TRUE, (const GLfloat *) p.getTransformation());

    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);

    glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, nullptr);

    glDisableVertexAttribArray(0);

    glutSwapBuffers();
}

void initializeGlutCallbacks() {
    // Задаём функции отрисовки
    glutDisplayFunc(renderSceneCallback);
    glutIdleFunc(renderSceneCallback);
}

void createVertexBuffer() {
    glm::vec3 vertices[4];

    vertices[0] = glm::vec3(-1.0f, -1.0f, 0.5773f);
    vertices[1] = glm::vec3(0.0f, -1.0f, -1.15475f);
    vertices[2] = glm::vec3(1.0f, -1.0f, 0.5773f);
    vertices[3] = glm::vec3(0.0f, 1.0f, 0.0f);

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
}

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

std::string readFile(const std::string &filePath) {
    std::string content;
    std::ifstream fileStream(filePath, std::ios::in);

    if (!fileStream.is_open()) {
        std::cerr << "Error: could not read file " << filePath << ". File does not exist" << std::endl;
        exit(1);
    }

    std::string line;
    while (!fileStream.eof()) {
        std::getline(fileStream, line);
        content.append(line + "\n");
    }

    fileStream.close();
    return content;
}

// Функция, добавляющая шейдер к программе
void addShader(GLuint shaderProgram, const std::string &pShaderText, GLenum shaderType) {
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
    lengths[0] = static_cast<GLint>(pShaderText.length());

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
    const auto pVS = readFile("./vertex.glsl");
    addShader(shaderProgram, pVS, GL_VERTEX_SHADER);
    // Добавляем шейдер для фрагментов
    const auto pFS = readFile("./fragment.glsl");
    addShader(shaderProgram, pFS, GL_FRAGMENT_SHADER);

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
    glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Tutorial 12");

    // Инициализируем callback-функции
    initializeGlutCallbacks();

    GLenum res = glewInit();
    if (res != GLEW_OK) {
        std::cerr << "Error glew init: '" << glewGetErrorString(res) << "'" << std::endl;
        return 1;
    }

    glClearColor(0.5f, 0.5f, 0.5f, 0.0f);

    createVertexBuffer();

    createIndexBuffer();

    compileShaders();

    glutMainLoop();

    return 0;
}
