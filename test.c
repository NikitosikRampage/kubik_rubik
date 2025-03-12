#include <stdlib.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <cglm/cglm.h>

// Структура для хранения позиций кубиков и цвета их граней 
typedef struct
{
    vec3 position;
    vec3 colors[7];
} Cube_Information;

#define CUBES_COUNT 64

GLFWwindow* window;
GLuint prog_line;
GLuint prog;

float scale = 1.0f;
float angleX = 0.0f;
float angleY = 0.0f;
/*тут надо допилить тему, что когда меняешь ориентацию, кнопки начинают путаться и вращать в другую сторону
подумать над углами надо*/
void processKey() { // Вращение кубика через клавиши
    const float rotationSpeed = 0.1f; // Скорость вращения

    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        angleX += rotationSpeed;
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        angleX -= rotationSpeed;
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        angleY += rotationSpeed;
    }
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        angleY -= rotationSpeed;
    }
}
// Callback-функция для обработки событий прокрутки колесика мыши
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
    // Изменяем радиус в зависимости от направления прокрутки
    if (yoffset > 0) {
        scale *= 0.9f;// Приближаем камеру
        if (scale < 0.25f) {
            // ограничеваю приблежение к кубу , надо подумать, 
            // тут можно более красиво реализовать, но вроде и так пока норм
            scale = 0.25f;
        }
    }
    else {
        scale *= 1.1f;// Отдаляем камеру
        if (scale > 3.0f) { // ограничеваю отдаление от куба
            scale = 3.0f;
        }
    }
}


int main(void) {
    // Инициализация GLFW
    if (!glfwInit()) {
        fprintf(stderr, "Failed to initialize GLFW\n");
        getchar();
        return -1;
    }

    // Настройка GLFW
    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Создание окна
    window = glfwCreateWindow(1920, 1080, "Rubik's Cube 4x4", NULL, NULL);
    if (window == NULL) {
        fprintf(stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n");
        getchar();
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    // Инициализация GLAD
    if (!gladLoadGL()) {
        fprintf(stderr, "Failed to initialize GLAD\n");
        glfwTerminate();
        return -1;
    }

    // Настройка OpenGL
    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glEnable(GL_DEPTH_TEST);

    // Вершины куба
    float cubeVertices[] = {
        // Передняя грань (красный)
        -0.5f, -0.5f, -0.5f,
         0.5f, -0.5f, -0.5f,
         0.5f,  0.5f, -0.5f,
        -0.5f,  0.5f, -0.5f,

        // Задняя грань (оранжевый)
        -0.5f, -0.5f,  0.5f,
         0.5f, -0.5f,  0.5f,
         0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f,  0.5f,

        // Левая грань (синий)
        -0.5f, -0.5f, -0.5f,
        -0.5f, -0.5f,  0.5f,
        -0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f, -0.5f,

        // Правая грань (зеленый)
         0.5f, -0.5f, -0.5f,
         0.5f, -0.5f,  0.5f,
         0.5f,  0.5f,  0.5f,
         0.5f,  0.5f, -0.5f,

         // Верхняя грань (белый)
         -0.5f,  0.5f, -0.5f,
          0.5f,  0.5f, -0.5f,
          0.5f,  0.5f,  0.5f,
         -0.5f,  0.5f,  0.5f,

         // Нижняя грань (желтый)
         -0.5f, -0.5f, -0.5f,
          0.5f, -0.5f, -0.5f,
          0.5f, -0.5f,  0.5f,
         -0.5f, -0.5f,  0.5f,
    };

    // Индексы куба
    unsigned int cube_and_lines_Indices[] = {
        // Передняя грань (треугольники)
        0, 1, 2,
        2, 3, 0,

        // Задняя грань (треугольники)
        4, 5, 6,
        6, 7, 4,

        // Левая грань (треугольники)
        8, 9, 10,
        10, 11, 8,

        // Правая грань (треугольники)
        12, 13, 14,
        14, 15, 12,

        // Верхняя грань (треугольники)
        16, 17, 18,
        18, 19, 16,

        // Нижняя грань (треугольники)
        20, 21, 22,
        22, 23, 20,

        // Линии: передняя грань
        0, 1,
        1, 2,
        2, 3,
        3, 0,

        // Линии: задняя грань
        4, 5,
        5, 6,
        6, 7,
        7, 4,

        // Линии: левая грань
        8, 9,
        9, 10,
        10, 11,
        11, 8,

        // Линии: правая грань
        12, 13,
        13, 14,
        14, 15,
        15, 12,

        // Линии: верхняя грань
        16, 17,
        17, 18,
        18, 19,
        19, 16,

        // Линии: нижняя грань
        20, 21,
        21, 22,
        22, 23,
        23, 20
    };

    vec3 red = { 1.0f, 0.0f, 0.0f }; // Красный
    vec3 green = { 0.0f, 1.0f, 0.0f }; // Зелёный
    vec3 blue = { 0.0f, 0.0f, 1.0f }; // Синий 
    vec3 white = { 1.0f, 1.0f, 1.0f }; // Белый
    vec3 orange = { 1.0f, 0.5f, 0.0f }; // Оранжевый
    vec3 yellow = { 1.0f, 1.0f, 0.0f };   // Жёлтый 
    vec3 grey = { 0.5f, 0.5f, 0.5f };  // Серый

    Cube_Information Cube[CUBES_COUNT];

    unsigned int Cube_Number = 0;
    for (int x = 0; x < 4; x++) {
        for (int y = 0; y < 4; y++) {
            for (int z = 0; z < 4; z++) {
                // Задаём координаты каждого куба
                Cube[Cube_Number].position[0] = x - 0.5f;
                Cube[Cube_Number].position[1] = y - 0.5f;
                Cube[Cube_Number].position[2] = z - 0.5f;

                for (int Gran = 0; Gran < 6; Gran++) {
                    memcpy(Cube[Cube_Number].colors[Gran], grey, sizeof(vec3));
                }

                // Раскрашиваем внешние грани
                if (x == 0) memcpy(Cube[Cube_Number].colors[1], orange, sizeof(vec3)); // Левая грань
                if (x == 3) memcpy(Cube[Cube_Number].colors[0], red, sizeof(vec3));    // Правая грань
                if (y == 0) memcpy(Cube[Cube_Number].colors[2], blue, sizeof(vec3));   // Нижняя грань
                if (y == 3) memcpy(Cube[Cube_Number].colors[3], green, sizeof(vec3));  // Верхняя грань
                if (z == 3) memcpy(Cube[Cube_Number].colors[4], white, sizeof(vec3));  // Передняя грань
                if (z == 0) memcpy(Cube[Cube_Number].colors[5], yellow, sizeof(vec3)); // Задняя грань

                Cube_Number++; // Увеличиваем номер кубика
            }
        }
    }



    // Создание VAO, VBO и EBO
    unsigned int VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    // Загрузка данных вершин в VBO
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), cubeVertices, GL_STATIC_DRAW);

    // Загрузка индексов в EBO
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cube_and_lines_Indices), cube_and_lines_Indices, GL_STATIC_DRAW);

    // Настройка атрибутов вершин
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);


    // Отвязываем VAO
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);


    //Компиляция шейдеров для Куба
    prog = Shader_CreateProgramm("vert.vs", "frag_cube.fs");
    if (!prog) {
        fprintf(stderr, "Failed to create shader program\n");
        glfwTerminate();
        return -1;
    }

    //Компиляция шйедеров для Линий
    prog_line = Shader_CreateProgramm("vert.vs", "frag_line.fs");
    if (!prog_line) {
        fprintf(stderr, "Failed to create shader program\n");
        glfwTerminate();
        return -1;
    }

    // Получение location uniform-переменных для кубов
    glUseProgram(prog);
    GLint modelLoc = glGetUniformLocation(prog, "model");
    GLint viewLoc = glGetUniformLocation(prog, "view");
    GLint projectionLoc = glGetUniformLocation(prog, "projection");

    if (modelLoc == -1 || viewLoc == -1 || projectionLoc == -1) {
        fprintf(stderr, "Error: Failed to get uniform locations\n");
        return -1;
    }

    // Получение location uniform-переменных для линий
    glUseProgram(prog_line);
    GLint modelLocLine = glGetUniformLocation(prog_line, "model");
    GLint viewLocLine = glGetUniformLocation(prog_line, "view");
    GLint projectionLocLine = glGetUniformLocation(prog_line, "projection");

    if (modelLocLine == -1 || viewLocLine == -1 || projectionLocLine == -1) {
        fprintf(stderr, "Error: Failed to get uniform locations for prog_line\n");
        return -1;
    }

    // функции для обработки мышки
    // Регистрация callback-функции для обработки событий прокрутки колесика мыши
      glfwSetScrollCallback(window, scroll_callback);

    // Основной цикл рендеринга
    while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
        glfwWindowShouldClose(window) == 0) {
        // Очистка буферов
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        processKey();

        // Создание и обновление матриц view и projection
        mat4 view, projection;
        vec3 eye = { 3.0f, 3.0f, 3.0f*scale }; // Камера смотрит на кубик Рубика
        vec3 center = { 0.0f, 0.0f, 0.0f }; // Центр кубика Рубика
        vec3 up = { 0.0f, 1.0f, 0.0f };
        glm_lookat(eye, center, up, view);

        float fov = glm_rad(100.0f);
        float aspect = 1920.0f / 1080.0f;
        float near = 0.1f;
        float far = 100.0f;
        glm_perspective(fov, aspect, near, far, projection);

        // Передаем view и projection в шейдер для линий
        glUseProgram(prog_line);
        glUniformMatrix4fv(viewLocLine, 1, GL_FALSE, (float*)view);
        glUniformMatrix4fv(projectionLocLine, 1, GL_FALSE, (float*)projection);

        // Передаем view и projection в шейдер для кубов
        glUseProgram(prog);
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, (float*)view);
        glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, (float*)projection);

        // Отрисовка кубиков
        for (int i = 0; i < CUBES_COUNT; i++) {
            // Создаем матрицу модели для текущего кубика
            mat4 model;
            glm_mat4_identity(model);
            
            glm_translate(model, (vec3) { -1.0f, -1.0f, -1.0f });
            // Применение вращения, крутим вокруг всех осей так хотябы он стоит на месте не дергается
            glm_rotate(model, glm_rad(angleY), (vec3) { 1.0f, 0.0f, 0.0f }); // Вращение по X
            glm_rotate(model, glm_rad(angleX), (vec3) { 0.0f, 1.0f, 0.0f }); // Вращение по Y

            glm_translate(model, (vec3) { -1.0f, -1.0f, - 1.0f });


            // Применяем трансляцию к матрице модели
            glm_translate(model, (vec3) { Cube[i].position[0], Cube[i].position[1], Cube[i].position[2] });




            // Передаем матрицу модели в шейдер для линий
            glLineWidth(3.0f);
            glDisable(GL_DEPTH_TEST);

            glUseProgram(prog_line);
            glUniformMatrix4fv(modelLocLine, 1, GL_FALSE, (float*)model);
            glEnable(GL_DEPTH_TEST);
            // Отрисовываем линии
            glBindVertexArray(VAO);
            glDrawElements(GL_LINES, 48, GL_UNSIGNED_INT, (void*)(36 * sizeof(unsigned int)));

            // Передаем матрицу модели в шейдер для кубов
            glUseProgram(prog);
            glUniformMatrix4fv(modelLoc, 1, GL_FALSE, (float*)model);

            glUniform3fv(glGetUniformLocation(prog, "Gran_Color"), 6, (float*)Cube[i].colors);

            // Отрисовываем куб
            glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);


        }
        
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Освобождение ресурсов
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glDeleteProgram(prog);
    glDeleteProgram(prog_line);

    glfwTerminate();
    return 0;
}