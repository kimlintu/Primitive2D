#version 330 core

in vec2 fTexCoord; 
out vec4 FragColor;

uniform sampler2D fTexture;

void main() {
    FragColor = texture(fTexture, fTexCoord);
    //FragColor = vec4(0.3f, 0.4f, 0.5f, 1.0f);
}
