#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor_Of_Cubes;
layout (location = 2) in vec3 aColor_Of_Lines;

out vec3 Cubes_Color;
out vec3 Lines_Color;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    gl_Position = projection * view * model * vec4(aPos, 1.0);
    Cubes_Color = aColor_Of_Cubes;
    Lines_Color = aColor_Of_Lines;
}