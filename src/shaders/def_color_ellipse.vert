#version 330 core

layout (location = 0) in vec2 inPos;

uniform mat4 projection;
uniform mat4 model;

void main() {
    gl_Position = projection * model * vec4(inPos.x, inPos.y, 0.0f, 1.0f);
}
