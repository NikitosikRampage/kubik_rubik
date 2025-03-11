#version 330 core
in vec3 Cubes_Color;
out vec4 FragColor;

void main()
{
    FragColor = vec4(Cubes_Color, 1.0);
}