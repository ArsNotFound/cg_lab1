#include <GL/glew.h>
#include <GL/freeglut.h>
#include <glm/vec3.hpp>
#include <iostream>

// Хранит указатель на буфер вершин
GLuint VBO;

int main(int argc, char **argv) {
    // Инициализация glut
    glutInit(&argc, argv);

    // Установка двойной буферизации и отображения в режиме RGBA
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);

    // Задание размера окна приложения (ширина, высота)
    glutInitWindowSize(1024, 768);

    // Начальная позиция окна (x и y относительно левого верхнего окна)
    glutInitWindowPosition(100, 100);

    // Создаём окно с названием "Tutorial 02"
    glutCreateWindow("Tutorial 02");

    // Инициализируем glew
    GLenum res = glewInit();
    // Если инициализация не прошла удачно, выводим ошибку
    if (res != GLEW_OK) {
        std::cout << "Error: " << glewGetErrorString(res) << std::endl;
        return 1;
    }

    // Создаём вектор вершин (векторов из glm)
    glm::vec3 vertices[3];
    // Создаём вектора (x, y, z относительно центра экрана)
    vertices[0] = glm::vec3(-1.0f, -1.0f, 0.0f);
    vertices[1] = glm::vec3(1.0f, -1.0f, 0.0f);
    vertices[2] = glm::vec3(0.0f, 1.0f, 0.0f);

    // Генерируем объект переменного типа (количество объектов, ссылка на массив для хранения)
    glGenBuffers(1, &VBO);
    // Указываем OpenGL, что в VBO будет храниться массив вершин (цель, указатель на массив)
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    // Наполняем буфер (цель, размер в байтах, адрес массива вершин, флаг, обозначающий использование
    // паттернов для данных)
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Задаём функцию отрисовки
    glutDisplayFunc([]() {
        // Очищаем окно
        glClear(GL_COLOR_BUFFER_BIT);

        // Разрешаем использование каждого атрибута вершины в конвейере (аттрибут вершины)
        glEnableVertexAttribArray(0);
        // Обратно привязываем буфер, приготавливая для отрисовки
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        // Говорим конвейеру, как воспринимать данные внутри буфера (индекс атрибута, количество аттрибутов,
        // тип данных каждого компонента, нормализировать ли данные перед использованием, шаг - число байтов
        // между двумя экземплярами атрибута, смещение первого компонента первого универсального атрибута вершины)
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

        // Вызываем функцию отрисовки (режим рисования, индекс первого элемента в буфере, количество элементов)
        glDrawArrays(GL_TRIANGLES, 0, 3);

        // Отключаем использование каждого атрибута вершины в конвейере [хороший тон]
        glDisableVertexAttribArray(0);

        // Меняем фоновый буфер и буфер кадра местами
        glutSwapBuffers();
    });

    // Запускаем основной цикл glut
    glutMainLoop();

    return 0;
}
