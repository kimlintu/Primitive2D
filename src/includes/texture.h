#ifndef TEXTURE_H
#define TEXTURE_H

#include "glad_i.h"
#include "sdl_gl.h"

struct Texture {
    GLuint id;
};

Texture load_texture(const char *src);

#endif
