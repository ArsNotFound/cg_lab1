// Версия шейдера 3.3
#version 330
// Входной тип данных - vec3 в позиции 0
layout (location = 0) in vec3 Position;
// uniform-переменная типа mat4
uniform mat4 gWorld;
// Выходной тип данных - vec4
out vec4 Color;

void main() {
    // Умножаем вектор вершин на всемирную матрицу для смещения треугольника
    gl_Position = gWorld * vec4(Position, 1.0);
    Color = vec4(clamp(Position, 0.0, 1.0), 1.0);
}
