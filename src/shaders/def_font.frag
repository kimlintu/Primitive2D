#version 330 core

in vec2 fTexCoord; 
out vec4 FragColor;

uniform sampler2D fTexture;
uniform float unfTexOffset;

void main() {
    ivec2 texSize = textureSize(fTexture, 0);

    float texCoordX = ((unfTexOffset * 64.0f) / texSize.x) + fTexCoord.x / 4.0f; 
    FragColor = texture(fTexture, vec2(texCoordX, fTexCoord.y / 4));
}
