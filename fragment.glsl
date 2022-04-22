// Версия шейдера 3.3
#version 330

// Входной тип данных - vec4
in vec4 Color;
// Выходной тип данных - vec4
out vec4 FragColor;

void main() {
    // Устанавливаем цвет
    FragColor = Color;
}

