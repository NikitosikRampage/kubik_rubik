#include <stdlib.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <cglm/cglm.h>

// ��������� ��� �������� ������� ������� � ����� �� ������ 
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
/*��� ���� �������� ����, ��� ����� ������� ����������, ������ �������� �������� � ������� � ������ �������
�������� ��� ������ ����*/
void processKey() { // �������� ������ ����� �������
    const float rotationSpeed = 0.1f; // �������� ��������

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
// Callback-������� ��� ��������� ������� ��������� �������� ����
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
    // �������� ������ � ����������� �� ����������� ���������
    if (yoffset > 0) {
        scale *= 0.9f;// ���������� ������
        if (scale < 0.25f) {
            // ����������� ����������� � ���� , ���� ��������, 
            // ��� ����� ����� ������� �����������, �� ����� � ��� ���� ����
            scale = 0.25f;
        }
    }
    else {
        scale *= 1.1f;// �������� ������
        if (scale > 3.0f) { // ����������� ��������� �� ����
            scale = 3.0f;
        }
    }
}


int main(void) {
    // ������������� GLFW
    if (!glfwInit()) {
        fprintf(stderr, "Failed to initialize GLFW\n");
        getchar();
        return -1;
    }

    // ��������� GLFW
    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // �������� ����
    window = glfwCreateWindow(1920, 1080, "Rubik's Cube 4x4", NULL, NULL);
    if (window == NULL) {
        fprintf(stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n");
        getchar();
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    // ������������� GLAD
    if (!gladLoadGL()) {
        fprintf(stderr, "Failed to initialize GLAD\n");
        glfwTerminate();
        return -1;
    }

    // ��������� OpenGL
    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glEnable(GL_DEPTH_TEST);

    // ������� ����
    float cubeVertices[] = {
        // �������� ����� (�������)
        -0.5f, -0.5f, -0.5f,
         0.5f, -0.5f, -0.5f,
         0.5f,  0.5f, -0.5f,
        -0.5f,  0.5f, -0.5f,

        // ������ ����� (���������)
        -0.5f, -0.5f,  0.5f,
         0.5f, -0.5f,  0.5f,
         0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f,  0.5f,

        // ����� ����� (�����)
        -0.5f, -0.5f, -0.5f,
        -0.5f, -0.5f,  0.5f,
        -0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f, -0.5f,

        // ������ ����� (�������)
         0.5f, -0.5f, -0.5f,
         0.5f, -0.5f,  0.5f,
         0.5f,  0.5f,  0.5f,
         0.5f,  0.5f, -0.5f,

         // ������� ����� (�����)
         -0.5f,  0.5f, -0.5f,
          0.5f,  0.5f, -0.5f,
          0.5f,  0.5f,  0.5f,
         -0.5f,  0.5f,  0.5f,

         // ������ ����� (������)
         -0.5f, -0.5f, -0.5f,
          0.5f, -0.5f, -0.5f,
          0.5f, -0.5f,  0.5f,
         -0.5f, -0.5f,  0.5f,
    };

    // ������� ����
    unsigned int cube_and_lines_Indices[] = {
        // �������� ����� (������������)
        0, 1, 2,
        2, 3, 0,

        // ������ ����� (������������)
        4, 5, 6,
        6, 7, 4,

        // ����� ����� (������������)
        8, 9, 10,
        10, 11, 8,

        // ������ ����� (������������)
        12, 13, 14,
        14, 15, 12,

        // ������� ����� (������������)
        16, 17, 18,
        18, 19, 16,

        // ������ ����� (������������)
        20, 21, 22,
        22, 23, 20,

        // �����: �������� �����
        0, 1,
        1, 2,
        2, 3,
        3, 0,

        // �����: ������ �����
        4, 5,
        5, 6,
        6, 7,
        7, 4,

        // �����: ����� �����
        8, 9,
        9, 10,
        10, 11,
        11, 8,

        // �����: ������ �����
        12, 13,
        13, 14,
        14, 15,
        15, 12,

        // �����: ������� �����
        16, 17,
        17, 18,
        18, 19,
        19, 16,

        // �����: ������ �����
        20, 21,
        21, 22,
        22, 23,
        23, 20
    };

    vec3 red = { 1.0f, 0.0f, 0.0f }; // �������
    vec3 green = { 0.0f, 1.0f, 0.0f }; // ������
    vec3 blue = { 0.0f, 0.0f, 1.0f }; // ����� 
    vec3 white = { 1.0f, 1.0f, 1.0f }; // �����
    vec3 orange = { 1.0f, 0.5f, 0.0f }; // ���������
    vec3 yellow = { 1.0f, 1.0f, 0.0f };   // Ƹ���� 
    vec3 grey = { 0.5f, 0.5f, 0.5f };  // �����

    Cube_Information Cube[CUBES_COUNT];

    unsigned int Cube_Number = 0;
    for (int x = 0; x < 4; x++) {
        for (int y = 0; y < 4; y++) {
            for (int z = 0; z < 4; z++) {
                // ����� ���������� ������� ����
                Cube[Cube_Number].position[0] = x - 0.5f;
                Cube[Cube_Number].position[1] = y - 0.5f;
                Cube[Cube_Number].position[2] = z - 0.5f;

                for (int Gran = 0; Gran < 6; Gran++) {
                    memcpy(Cube[Cube_Number].colors[Gran], grey, sizeof(vec3));
                }

                // ������������ ������� �����
                if (x == 0) memcpy(Cube[Cube_Number].colors[1], orange, sizeof(vec3)); // ����� �����
                if (x == 3) memcpy(Cube[Cube_Number].colors[0], red, sizeof(vec3));    // ������ �����
                if (y == 0) memcpy(Cube[Cube_Number].colors[2], blue, sizeof(vec3));   // ������ �����
                if (y == 3) memcpy(Cube[Cube_Number].colors[3], green, sizeof(vec3));  // ������� �����
                if (z == 3) memcpy(Cube[Cube_Number].colors[4], white, sizeof(vec3));  // �������� �����
                if (z == 0) memcpy(Cube[Cube_Number].colors[5], yellow, sizeof(vec3)); // ������ �����

                Cube_Number++; // ����������� ����� ������
            }
        }
    }



    // �������� VAO, VBO � EBO
    unsigned int VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    // �������� ������ ������ � VBO
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), cubeVertices, GL_STATIC_DRAW);

    // �������� �������� � EBO
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cube_and_lines_Indices), cube_and_lines_Indices, GL_STATIC_DRAW);

    // ��������� ��������� ������
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);


    // ���������� VAO
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);


    //���������� �������� ��� ����
    prog = Shader_CreateProgramm("vert.vs", "frag_cube.fs");
    if (!prog) {
        fprintf(stderr, "Failed to create shader program\n");
        glfwTerminate();
        return -1;
    }

    //���������� �������� ��� �����
    prog_line = Shader_CreateProgramm("vert.vs", "frag_line.fs");
    if (!prog_line) {
        fprintf(stderr, "Failed to create shader program\n");
        glfwTerminate();
        return -1;
    }

    // ��������� location uniform-���������� ��� �����
    glUseProgram(prog);
    GLint modelLoc = glGetUniformLocation(prog, "model");
    GLint viewLoc = glGetUniformLocation(prog, "view");
    GLint projectionLoc = glGetUniformLocation(prog, "projection");

    if (modelLoc == -1 || viewLoc == -1 || projectionLoc == -1) {
        fprintf(stderr, "Error: Failed to get uniform locations\n");
        return -1;
    }

    // ��������� location uniform-���������� ��� �����
    glUseProgram(prog_line);
    GLint modelLocLine = glGetUniformLocation(prog_line, "model");
    GLint viewLocLine = glGetUniformLocation(prog_line, "view");
    GLint projectionLocLine = glGetUniformLocation(prog_line, "projection");

    if (modelLocLine == -1 || viewLocLine == -1 || projectionLocLine == -1) {
        fprintf(stderr, "Error: Failed to get uniform locations for prog_line\n");
        return -1;
    }

    // ������� ��� ��������� �����
    // ����������� callback-������� ��� ��������� ������� ��������� �������� ����
      glfwSetScrollCallback(window, scroll_callback);

    // �������� ���� ����������
    while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
        glfwWindowShouldClose(window) == 0) {
        // ������� �������
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        processKey();

        // �������� � ���������� ������ view � projection
        mat4 view, projection;
        vec3 eye = { 3.0f, 3.0f, 3.0f*scale }; // ������ ������� �� ����� ������
        vec3 center = { 0.0f, 0.0f, 0.0f }; // ����� ������ ������
        vec3 up = { 0.0f, 1.0f, 0.0f };
        glm_lookat(eye, center, up, view);

        float fov = glm_rad(100.0f);
        float aspect = 1920.0f / 1080.0f;
        float near = 0.1f;
        float far = 100.0f;
        glm_perspective(fov, aspect, near, far, projection);

        // �������� view � projection � ������ ��� �����
        glUseProgram(prog_line);
        glUniformMatrix4fv(viewLocLine, 1, GL_FALSE, (float*)view);
        glUniformMatrix4fv(projectionLocLine, 1, GL_FALSE, (float*)projection);

        // �������� view � projection � ������ ��� �����
        glUseProgram(prog);
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, (float*)view);
        glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, (float*)projection);

        // ��������� �������
        for (int i = 0; i < CUBES_COUNT; i++) {
            // ������� ������� ������ ��� �������� ������
            mat4 model;
            glm_mat4_identity(model);
            
            glm_translate(model, (vec3) { -1.0f, -1.0f, -1.0f });
            // ���������� ��������, ������ ������ ���� ���� ��� ������ �� ����� �� ����� �� ���������
            glm_rotate(model, glm_rad(angleY), (vec3) { 1.0f, 0.0f, 0.0f }); // �������� �� X
            glm_rotate(model, glm_rad(angleX), (vec3) { 0.0f, 1.0f, 0.0f }); // �������� �� Y

            glm_translate(model, (vec3) { -1.0f, -1.0f, - 1.0f });


            // ��������� ���������� � ������� ������
            glm_translate(model, (vec3) { Cube[i].position[0], Cube[i].position[1], Cube[i].position[2] });




            // �������� ������� ������ � ������ ��� �����
            glLineWidth(3.0f);
            glDisable(GL_DEPTH_TEST);

            glUseProgram(prog_line);
            glUniformMatrix4fv(modelLocLine, 1, GL_FALSE, (float*)model);
            glEnable(GL_DEPTH_TEST);
            // ������������ �����
            glBindVertexArray(VAO);
            glDrawElements(GL_LINES, 48, GL_UNSIGNED_INT, (void*)(36 * sizeof(unsigned int)));

            // �������� ������� ������ � ������ ��� �����
            glUseProgram(prog);
            glUniformMatrix4fv(modelLoc, 1, GL_FALSE, (float*)model);

            glUniform3fv(glGetUniformLocation(prog, "Gran_Color"), 6, (float*)Cube[i].colors);

            // ������������ ���
            glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);


        }
        
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // ������������ ��������
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glDeleteProgram(prog);
    glDeleteProgram(prog_line);

    glfwTerminate();
    return 0;
}