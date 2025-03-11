#version 330 core
in vec3 Lines_Color;
out vec4 FragColor;

void main()
{
    FragColor = vec4(Lines_Color, 1.0);
}