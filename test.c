#include <stdlib.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <cglm/cglm.h>

#define CUBES_COUNT 64

typedef enum
{
    middle_middle,
    left_middle,
    right_middle,
    up_middle,
    down_middle,
    left_up_corner,
    right_up_corner,
    left_down_corner,
    right_down_corner,
} individual_cube_position;

// Структура для хранения позиций кубиков и цвета их граней
typedef struct
{
    vec3 coordinates;
    vec3 colors_edge[6];
} model_cube_information;

typedef struct
{
    vec2 coordinates;
    vec3 color;
    float postions_of_sqaure[3];
    individual_cube_position position_type;
} square_information;

typedef union
{
    model_cube_information model;
    square_information square;
}individual_cube_color;

typedef struct 
{
    individual_cube_color* cubes;
    GLuint VBO;
    vec3 current_color;
    float square_size;
} cross;

// Обработка вращения кубика с помощью клавиш
void key_rotate(float *angleX,float *angleY, GLFWwindow* window) {
    const float rotationSpeed = 0.2f; // Скорость вращения

    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        *angleX += rotationSpeed;
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        *angleX -= rotationSpeed;
    }
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        *angleY += rotationSpeed;
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        *angleY -= rotationSpeed;
    }
}

// Обработчик кликов мыши
void mouse_color(GLFWwindow* window, int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
        cross* data = (cross*)glfwGetWindowUserPointer(window);

        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);

        int width, height;
        glfwGetFramebufferSize(window, &width, &height);

        // Преобразование координат для проекции [-2, 2]
        float x = -2.0f + (xpos / width) * 4.0f;
        float y = 2.0f - (ypos / height) * 4.0f;

        for (int i = 0; i < 96; i++) {
            float left = data->cubes[i].square.coordinates[0] - data->square_size / 2;
            float right = data->cubes[i].square.coordinates[0] + data->square_size / 2;
            float top = data->cubes[i].square.coordinates[1] + data->square_size / 2;
            float bottom = data->cubes[i].square.coordinates[1] - data->square_size / 2;

            if (x >= left && x <= right && y >= bottom && y <= top) {
                // Копируем текущий цвет в квадрат
                glm_vec3_copy(data->current_color, data->cubes[i].square.color);

                // Обновляем только цвет в буфере
                glBindBuffer(GL_ARRAY_BUFFER, data->VBO);
                glBufferSubData(GL_ARRAY_BUFFER,
                    i * sizeof(individual_cube_color) + offsetof(individual_cube_color, square.color),
                    sizeof(vec3),
                    data->cubes[i].square.color);
                break;
            }
        }
    }
}
void key_color(GLFWwindow* window, cross* data) 
{
    if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
        glm_vec3_copy((vec3) { 1.0f, 0.0f, 0.0f }, data->current_color); // Красный
    if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
        glm_vec3_copy((vec3) { 0.0f, 1.0f, 0.0f }, data->current_color); // Зеленый
    if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS)
        glm_vec3_copy((vec3) { 0.0f, 0.0f, 1.0f }, data->current_color); // Синий
    if (glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS)
        glm_vec3_copy((vec3) { 1.0f, 1.0f, 1.0f }, data->current_color); // Белый
    if (glfwGetKey(window, GLFW_KEY_5) == GLFW_PRESS)
        glm_vec3_copy((vec3) { 1.0f, 0.5f, 0.0f }, data->current_color); // Оранжевый
    if (glfwGetKey(window, GLFW_KEY_6) == GLFW_PRESS)
        glm_vec3_copy((vec3) { 1.0f, 1.0f, 0.0f }, data->current_color); // Желтый
    if (glfwGetKey(window, GLFW_KEY_0) == GLFW_PRESS)
        glm_vec3_copy((vec3) { 0.5f, 0.5f, 0.5f }, data->current_color); // Серый
}
GLuint color_Cube()
{
    GLFWwindow* window;
    window = glfwCreateWindow(1920, 1080, "Color Cube 4x4", NULL, NULL);
    if (window == NULL) {
        fprintf(stderr, "Failed to open GLFW window.\n");
        return;
    }
    glfwMakeContextCurrent(window);

    if (!gladLoadGL()) {
        fprintf(stderr, "Failed to initialize GLAD\n");
        return;
    }

    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_MULTISAMPLE);

    individual_cube_color Squares[96];
    cross data = {
        .cubes = Squares,
        .current_color = {1.0f, 0.0f, 0.0f},
        .square_size = 0.18f
    };

    float squareVertices[] = {
        -0.5f,  0.5f,  0.0f, 1.0f,
         0.5f,  0.5f,  1.0f, 1.0f,
         0.5f, -0.5f,  1.0f, 0.0f,
        -0.5f, -0.5f,  0.0f, 0.0f
    };

    unsigned int indices[] = {
        0, 1, 2,
        2, 3, 0
    };

    float square_size = 0.18f;
    float space = 0.02f;
    float face_size = 4 * square_size + 3 * space;

    float base_x0 = -0.4f;
    float base_y0 = -0.4f;

    unsigned int Square_Number = 0;
    for (int face = 0; face < 6; face++) {
        for (int row = 0; row < 4; row++) {
            for (int col = 0; col < 4; col++) {
                float fx = base_x0 + col * (square_size + space);
                float fy = base_y0 + row * (square_size + space);

                if (face == 0) {
                    Squares[Square_Number].square.coordinates[0] = fx;
                    Squares[Square_Number].square.coordinates[1] = fy;
                }
                else if (face == 1) {
                    Squares[Square_Number].square.coordinates[0] = fx;
                    Squares[Square_Number].square.coordinates[1] = fy - (face_size + space);
                }
                else if (face == 2) {
                    Squares[Square_Number].square.coordinates[0] = fx;
                    Squares[Square_Number].square.coordinates[1] = fy + (face_size + space);
                }
                else if (face == 3) {
                    Squares[Square_Number].square.coordinates[0] = fx - (face_size + space);
                    Squares[Square_Number].square.coordinates[1] = fy;
                }
                else if (face == 4) {
                    Squares[Square_Number].square.coordinates[0] = fx + (face_size + space);
                    Squares[Square_Number].square.coordinates[1] = fy;
                }
                else if (face == 5) {
                    Squares[Square_Number].square.coordinates[0] = fx + 2 * (face_size + space);
                    Squares[Square_Number].square.coordinates[1] = fy;
                }
                glm_vec3_copy((vec3) { 0.5f, 0.5f, 0.5f }, Squares[Square_Number].square.color);
                Square_Number++;
            }
        }
    }

    GLuint prog_square = Shader_CreateProgramm("vert_square.vs", "frag_square.fs");
    if (!prog_square) {
        fprintf(stderr, "Failed to create shader program\n");
        return;
    }

    GLuint VAO, VBO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(squareVertices), squareVertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glGenBuffers(1, &data.VBO);
    glBindBuffer(GL_ARRAY_BUFFER, data.VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Squares), Squares, GL_DYNAMIC_DRAW);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(individual_cube_color), (void*)offsetof(individual_cube_color, square.coordinates));
    glEnableVertexAttribArray(1);
    glVertexAttribDivisor(1, 1);

    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(individual_cube_color), (void*)offsetof(individual_cube_color, square.color));
    glEnableVertexAttribArray(2);
    glVertexAttribDivisor(2, 1);

    glBindVertexArray(0);

    glfwSetWindowUserPointer(window, &data);
    glfwSetMouseButtonCallback(window, mouse_color);

    mat4 projection;
    glm_ortho(-2.0f, 2.0f, -2.0f, 2.0f, -1.0f, 1.0f, projection);
    glUseProgram(prog_square);
    glUniformMatrix4fv(glGetUniformLocation(prog_square, "projection"), 1, GL_FALSE, (float*)projection);

    while (!glfwWindowShouldClose(window)) {
        if (glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS) {
            glfwSetWindowShouldClose(window, GLFW_TRUE);
        }



        key_color(window, &data);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glBindVertexArray(VAO);
        glDrawElementsInstanced(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0, 96);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glDeleteBuffers(1, &data.VBO);
    glDeleteProgram(prog_square);

    glfwDestroyWindow(window);
    return 1;
}
void draw_Cube() {
    GLuint prog_line;
    GLuint prog;

    GLFWwindow* window;
    // Создание окна
    window = glfwCreateWindow(1920, 1080, "Rubik's Cube 4x4", NULL, NULL);
    if (window == NULL) {
        fprintf(stderr, "Failed to open GLFW window.\n");
        getchar();
        glfwTerminate();
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



    // Настройка OpenGL
    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glEnable(GL_DEPTH_TEST);

    glEnable(GL_MULTISAMPLE);

    // Вершины куба
    float cubeVertices[] = {
        // Передняя грань
        -0.5f, -0.5f, -0.5f,
         0.5f, -0.5f, -0.5f,
         0.5f,  0.5f, -0.5f,
        -0.5f,  0.5f, -0.5f,

        // Задняя грань
        -0.5f, -0.5f,  0.5f,
         0.5f, -0.5f,  0.5f,
         0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f,  0.5f,

        // Левая грань
        -0.5f, -0.5f, -0.5f,
        -0.5f, -0.5f,  0.5f,
        -0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f, -0.5f,

        // Правая грань
         0.5f, -0.5f, -0.5f,
         0.5f, -0.5f,  0.5f,
         0.5f,  0.5f,  0.5f,
         0.5f,  0.5f, -0.5f,

         // Верхняя грань
         -0.5f,  0.5f, -0.5f,
          0.5f,  0.5f, -0.5f,
          0.5f,  0.5f,  0.5f,
         -0.5f,  0.5f,  0.5f,

         // Нижняя грань
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
    vec3 green = { 0.0f, 1.0f, 0.0f }; // Зеленый
    vec3 blue = { 0.0f, 0.0f, 1.0f }; // Синий
    vec3 white = { 1.0f, 1.0f, 1.0f }; // Белый
    vec3 orange = { 1.0f, 0.5f, 0.0f }; // Оранжевый
    vec3 yellow = { 1.0f, 1.0f, 0.0f }; // Желтый
    vec3 grey = { 0.5f, 0.5f, 0.5f }; // Серый

    individual_cube_color Cube[CUBES_COUNT]; //Выделить динамически

    unsigned int Cube_Number = 0;
    for (int x = 0; x < 4; x++) {
        for (int y = 0; y < 4; y++) {
            for (int z = 0; z < 4; z++) {
                // Задаем координаты каждого куба
                Cube[Cube_Number].model.coordinates[0] = x - 0.5f;
                Cube[Cube_Number].model.coordinates[1] = y - 0.5f;
                Cube[Cube_Number].model.coordinates[2] = z - 0.5f;

                for (int Gran = 0; Gran < 6; Gran++) {
                    memcpy(Cube[Cube_Number].model.colors_edge[Gran], grey, sizeof(vec3));
                }

                // Раскрашиваем внешние грани
                if (x == 0) memcpy(Cube[Cube_Number].model.colors_edge[2], orange, sizeof(vec3));  // Левая грань
                if (x == 3) memcpy(Cube[Cube_Number].model.colors_edge[3], red, sizeof(vec3));     // Правая грань
                if (y == 0) memcpy(Cube[Cube_Number].model.colors_edge[5], blue, sizeof(vec3));   // Нижняя грань
                if (y == 3) memcpy(Cube[Cube_Number].model.colors_edge[4], green, sizeof(vec3));  // Верхняя грань
                if (z == 3) memcpy(Cube[Cube_Number].model.colors_edge[1], white, sizeof(vec3));  // Передняя грань
                if (z == 0) memcpy(Cube[Cube_Number].model.colors_edge[0], yellow, sizeof(vec3)); // Задняя грань

                Cube_Number++; // Увеличиваем номер кубика
            }
        }
    }

    // Создание VAO, VBO и EBO
    GLuint VBO, VAO, EBO;
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

    // Компиляция шейдеров для куба
    prog = Shader_CreateProgramm("vert.vs", "frag_cube.fs");
    if (!prog) {
        fprintf(stderr, "Failed to create shader program\n");
        glfwTerminate();
        return -1;
    }

    // Компиляция шейдеров для линий
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


    float angleX = 0.0f;
    float angleY = 0.0f;
    // Основной цикл рендеринга
    while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
        glfwWindowShouldClose(window) == 0) {

        int width, height;
        glfwGetFramebufferSize(window, &width, &height);
        glViewport(0, 0, width, height);

        // Очистка буферов
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        key_rotate(&angleX, &angleY, window);

        float aspect = (float)width / (float)height;

        // Создание и обновление матриц view и projection
        mat4 view, projection;
        vec3 eye = { 3.0f, 3.0f, 3.0f }; // Камера смотрит на кубик Рубика
        vec3 center = { -1.0f , -1.0f  ,-1.0f }; // Центр кубика Рубика
        vec3 up = { 0.0f , 1.0f ,0.0f };
        // Создаем матрицу вращения вокруг оси Y на 45 градусов
        mat4 rotation;
        glm_rotate_make(rotation, glm_rad(-60.05f), (vec3) { 0.0f, 1.0f, 0.0f });

        // Применяем вращение к вектору eye
        vec3 new_eye;
        glm_mat4_mulv3(rotation, eye, 1.0f, new_eye);

        // Обновляем матрицу view с новым положением камеры
        glm_lookat(new_eye, center, up, view);
        //glm_lookat(eye, center, up, view);

        float fov = glm_rad(100.0f);
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
            // glm_scale(model, (vec3) { 0.8f*scale, 0.8f*scale, 0.8f*scale });
            glm_translate(model, (vec3) { -1.0f, -1.0f, -1.0f });
            // Применение вращения
            glm_rotate(model, glm_rad(angleY), (vec3) { 1.0f, 0.0f, 0.0f }); // Вращение по X
            glm_rotate(model, glm_rad(angleX), (vec3) { 0.0f, 1.0f, 0.0f }); // Вращение по Y

            glm_translate(model, (vec3) { -1.0f, -1.0f, -1.0f });

            // Применяем трансляцию к матрице модели
            glm_translate(model, (vec3) { Cube[i].model.coordinates[0], Cube[i].model.coordinates[1], Cube[i].model.coordinates[2] });

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

            glUniform3fv(glGetUniformLocation(prog, "Gran_Color"), 6, (float*)Cube[i].model.colors_edge);

            // Отрисовываем куб
            glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glDeleteProgram(prog);
    glDeleteProgram(prog_line);

    glfwTerminate();
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

    color_Cube();
    draw_Cube();

    return 0;
}