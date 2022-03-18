# CG LAB 1
Первая лабораторная работа по компьютерной графике.

Выполнено:
- Установка необходимых библиотек OpegGL (freeglut, glew, glm)
```shell
paru -S freeglut glew glm
```
- Создание окна при помощи библиотеки glut
```c++
 // Инициализация glut
 glutInit(&argc, argv); 
 
 // Установка двойной буферизации и отображения в режиме RGBA
 glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
 
 // Задание размера окна приложения (ширина, высота)
 glutInitWindowSize(1024, 768);
 
 // Начальная позиция окна (относительно левого верхнего окна)
 glutInitWindowPosition(100, 100);
 
 // Создаём окно с названием "Tutorial 01"
 glutCreateWindow("Tutorial 01");

 // Задаём функцию отрисовки
 glutDisplayFunc([]() {
     // Очищаем окно
     glClear(GL_COLOR_BUFFER_BIT);
     
     // Меняем фоновый буфер и буфер кадра местами
     glutSwapBuffers();
 });
 
 // Запускаем основной цикл glut
 glutMainLoop();
```
