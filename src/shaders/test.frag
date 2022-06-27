#version 330 core

in vec2 fTexCoord; 
out vec4 FragColor;

uniform sampler2D fTexture;

void main() {
    FragColor = texture(fTexture, fTexCoord);
}
