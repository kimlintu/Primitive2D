#version 330 core

out vec4 FragColor;

uniform float unfRGBColor[3];

void main() {
    FragColor = vec4(unfRGBColor[0], unfRGBColor[1], unfRGBColor[2], 1.0f);
}
