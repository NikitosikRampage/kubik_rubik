#include <stdio.h>
#include <stdlib.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <cglm/cglm.h>

GLFWwindow* window;

GLuint prog;
GLuint prog_line;

int main(void)
{
    // ������������� GLFW
    if (!glfwInit())
    {
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
    window = glfwCreateWindow(1920, 1080, "Tutorial 01", NULL, NULL);
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

    // �������� ������
    float vertices[125][9];
    int index_of_vertices = 0;
    for (int x = 0; x < 5; x++)
    {
        for (int y = 0; y < 5; y++)
        {
            for (int z = 0; z < 5; z++)
            {
                // ���������� ������
                vertices[index_of_vertices][0] = x + 2.0f;
                vertices[index_of_vertices][1] = y + 2.0f;
                vertices[index_of_vertices][2] = z + 2.0f;

                // ���� �����
                vertices[index_of_vertices][3] = 1.0f;
                vertices[index_of_vertices][4] = 0.0f;
                vertices[index_of_vertices][5] = 0.0f;

                // ���� �������
                vertices[index_of_vertices][6] = 0.0f; 
                vertices[index_of_vertices][7] = 0.0f; 
                vertices[index_of_vertices][8] = 0.0f; 

                index_of_vertices++;
            }
        }
    }

    // �������� �������� ��� ����� � �����
    unsigned int cube_indices[3840];
    int index_of_cubes_and_lines = 0;

    // ������� ��� ����� (2304 �������)
    for (int x = 0; x < 4; x++) {
        for (int y = 0; y < 4; y++) {
            for (int z = 0; z < 4; z++) {
                int point1 = x * 25 + y * 5 + z;
                int point2 = point1 + 1;
                int point3 = point1 + 5;
                int point4 = point3 + 1;
                int point5 = point1 + 25;
                int point6 = point2 + 25;
                int point7 = point3 + 25;
                int point8 = point4 + 25;

                // �������� �����
                cube_indices[index_of_cubes_and_lines++] = point1; 
                cube_indices[index_of_cubes_and_lines++] = point2; 
                cube_indices[index_of_cubes_and_lines++] = point3;


                cube_indices[index_of_cubes_and_lines++] = point3;
                cube_indices[index_of_cubes_and_lines++] = point2;
                cube_indices[index_of_cubes_and_lines++] = point4;

                // ������ �����
                cube_indices[index_of_cubes_and_lines++] = point5;
                cube_indices[index_of_cubes_and_lines++] = point6;
                cube_indices[index_of_cubes_and_lines++] = point7;


                cube_indices[index_of_cubes_and_lines++] = point7;
                cube_indices[index_of_cubes_and_lines++] = point6;
                cube_indices[index_of_cubes_and_lines++] = point8;

                // ����� �����
                cube_indices[index_of_cubes_and_lines++] = point1; 
                cube_indices[index_of_cubes_and_lines++] = point3; 
                cube_indices[index_of_cubes_and_lines++] = point5;


                cube_indices[index_of_cubes_and_lines++] = point5;
                cube_indices[index_of_cubes_and_lines++] = point3; 
                cube_indices[index_of_cubes_and_lines++] = point7;

                // ������ �����
                cube_indices[index_of_cubes_and_lines++] = point2;
                cube_indices[index_of_cubes_and_lines++] = point4;
                cube_indices[index_of_cubes_and_lines++] = point6;


                cube_indices[index_of_cubes_and_lines++] = point6;
                cube_indices[index_of_cubes_and_lines++] = point4; 
                cube_indices[index_of_cubes_and_lines++] = point8;

                // ������� �����
                cube_indices[index_of_cubes_and_lines++] = point3;
                cube_indices[index_of_cubes_and_lines++] = point4; 
                cube_indices[index_of_cubes_and_lines++] = point7;


                cube_indices[index_of_cubes_and_lines++] = point7;
                cube_indices[index_of_cubes_and_lines++] = point4;
                cube_indices[index_of_cubes_and_lines++] = point8;

                // ������ �����
                cube_indices[index_of_cubes_and_lines++] = point1;
                cube_indices[index_of_cubes_and_lines++] = point2;
                cube_indices[index_of_cubes_and_lines++] = point5;


                cube_indices[index_of_cubes_and_lines++] = point5; 
                cube_indices[index_of_cubes_and_lines++] = point2;
                cube_indices[index_of_cubes_and_lines++] = point6;
            }
        }
    }

    // ������� ��� ����� (1536 ��������)
    for (int x = 0; x < 4; x++) {
        for (int y = 0; y < 4; y++) {
            for (int z = 0; z < 4; z++) {
                int point1 = x * 25 + y * 5 + z;
                int point2 = point1 + 1;
                int point3 = point1 + 5;
                int point4 = point3 + 1;
                int point5 = point1 + 25;
                int point6 = point2 + 25;
                int point7 = point3 + 25;
                int point8 = point4 + 25;

                // �������� �����
                cube_indices[index_of_cubes_and_lines++] = point1; 
                cube_indices[index_of_cubes_and_lines++] = point2;


                cube_indices[index_of_cubes_and_lines++] = point2; 
                cube_indices[index_of_cubes_and_lines++] = point4;


                cube_indices[index_of_cubes_and_lines++] = point4; 
                cube_indices[index_of_cubes_and_lines++] = point3;


                cube_indices[index_of_cubes_and_lines++] = point3;
                cube_indices[index_of_cubes_and_lines++] = point1;

                // ������ �����
                cube_indices[index_of_cubes_and_lines++] = point5; 
                cube_indices[index_of_cubes_and_lines++] = point6;


                cube_indices[index_of_cubes_and_lines++] = point6;
                cube_indices[index_of_cubes_and_lines++] = point8;


                cube_indices[index_of_cubes_and_lines++] = point8;
                cube_indices[index_of_cubes_and_lines++] = point7;


                cube_indices[index_of_cubes_and_lines++] = point7; 
                cube_indices[index_of_cubes_and_lines++] = point5;

                // ������� �����
                cube_indices[index_of_cubes_and_lines++] = point1; 
                cube_indices[index_of_cubes_and_lines++] = point5;


                cube_indices[index_of_cubes_and_lines++] = point2; 
                cube_indices[index_of_cubes_and_lines++] = point6;


                cube_indices[index_of_cubes_and_lines++] = point3; 
                cube_indices[index_of_cubes_and_lines++] = point7;


                cube_indices[index_of_cubes_and_lines++] = point4; 
                cube_indices[index_of_cubes_and_lines++] = point8;
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
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // �������� �������� � EBO
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cube_indices), cube_indices, GL_STATIC_DRAW);

    // ��������� ��������� ������
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // ��������� ��������� ����� �����
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // ��������� ��������� ����� �����
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    // ���������� VAO
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    // ���������� ��������
    prog = Shader_CreateProgramm("vert.vs", "frag_cube.fs");
    if (!prog) {
        fprintf(stderr, "Failed to create shader program\n");
        glfwTerminate();
        return -1;
    }

    prog_line = Shader_CreateProgramm("vert.vs", "frag_line.fs");
    if (!prog_line) {
        fprintf(stderr, "Failed to create shader program\n");
        glfwTerminate();
        return -1;
    }

    // ��������� location uniform-����������
    glUseProgram(prog);
    GLint modelLoc = glGetUniformLocation(prog, "model");
    GLint viewLoc = glGetUniformLocation(prog, "view");
    GLint projectionLoc = glGetUniformLocation(prog, "projection");

    if (modelLoc == -1 || viewLoc == -1 || projectionLoc == -1) {
        fprintf(stderr, "Error: Failed to get uniform locations\n");
        return -1;
    }

    glUseProgram(prog_line);
    GLint modelLocLine = glGetUniformLocation(prog_line, "model");
    GLint viewLocLine = glGetUniformLocation(prog_line, "view");
    GLint projectionLocLine = glGetUniformLocation(prog_line, "projection");

    if (modelLocLine == -1 || viewLocLine == -1 || projectionLocLine == -1) {
        fprintf(stderr, "Error: Failed to get uniform locations for prog_line\n");
        return -1;
    }

    // �������� ���� ����������
    while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
        glfwWindowShouldClose(window) == 0)
    {
        // ������� �������
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // �������� � ���������� ������
        mat4 model, view, projection;
        float angle = (float)glfwGetTime();
        glm_mat4_identity(model);
        glm_rotate(model, angle, (vec3) { 0.5f, 1.0f, 0.0f }); // ��������
        glm_scale(model, (vec3) { 0.1f, 0.1f, 0.1f }); // ���������������

        vec3 eye = { 0.0f, 0.0f, 3.0f };
        vec3 center = { 0.0f, 0.0f, 0.0f };
        vec3 up = { 0.0f, 1.0f, 0.0f };
        glm_lookat(eye, center, up, view);

        float fov = glm_rad(45.0f);
        float aspect = 1920.0f / 1080.0f;
        float near = 0.1f;
        float far = 100.0f;
        glm_perspective(fov, aspect, near, far, projection);

        // ��������� �����
        glUseProgram(prog);
        glEnable(GL_DEPTH_TEST);
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, (float*)model);
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, (float*)view);
        glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, (float*)projection);
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 2304, GL_UNSIGNED_INT, 0);

        // ��������� �����
        glUseProgram(prog_line);
        glUniformMatrix4fv(modelLocLine, 1, GL_FALSE, (float*)model);
        glUniformMatrix4fv(viewLocLine, 1, GL_FALSE, (float*)view);
        glUniformMatrix4fv(projectionLocLine, 1, GL_FALSE, (float*)projection);
        glDrawElements(GL_LINES, 1536, GL_UNSIGNED_INT, (void*)(2304 * sizeof(unsigned int)));

        // ���������� ����
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // ������������ ��������
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glDeleteProgram(prog);
    glDeleteProgram(prog_line);

	return 0;
}
