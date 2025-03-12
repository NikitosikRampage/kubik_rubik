#version 330 core
layout (location = 0) in vec3 aPos;

out vec3 Cubes_Color;
out vec3 Lines_Color;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform vec3 Gran_Color[6]; //���� �����

void main()
{
    gl_Position = projection * view * model * vec4(aPos, 1.0);
    int Gran_Index = gl_VertexID / 4; // ���������� ������ ������� � VBO, ������ ����� ������� �� 4 ������
    Cubes_Color = Gran_Color[Gran_Index];
}