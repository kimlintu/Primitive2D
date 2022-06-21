#ifndef GL_UTIL
#define GL_UTIL

#include "includes/glad_i.h"
#include "includes/sdl_gl.h"

#include "includes/vertex.h"

// TODO: rename?
void load_gl_buffers(Vertex *vertices, uint64_t n, GLuint *vbo, GLuint *vao);

#endif
