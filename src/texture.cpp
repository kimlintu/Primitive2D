#include "includes/texture.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image/stb_image.h>

Texture load_texture(const char *src) {
    Texture texture;
    glGenTextures(1, &texture.id);

    glBindTexture(GL_TEXTURE_2D, texture.id);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    int32_t width, height, nr_channels;
    uint8_t *data = stbi_load(src, &width, &height, &nr_channels, 0);
    if(data) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    } else {
        fprintf(stderr, "Could not load texture. Src: %s\n", src);
        fprintf(stderr, "stbi error: %s\n", stbi_failure_reason());
        glDeleteTextures(1, &texture.id);
        texture.id = 0;
    }

    stbi_image_free(data);

    glBindTexture(GL_TEXTURE_2D, 0);

    return texture;
}
