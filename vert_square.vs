#version 330 core
layout (location = 0) in vec2 position;
layout (location = 1) in vec2 instancePosition;
layout (location = 2) in vec3 instanceColor;

out vec3 fragColor;

uniform mat4 projection;

void main() {
    vec2 pos = position * 0.18; // Устанавливаем размер квадрата равным square_size
    gl_Position = projection * vec4(pos + instancePosition, 0.0, 1.0);
    fragColor = instanceColor;
}