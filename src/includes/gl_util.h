#ifndef GL_UTIL
#define GL_UTIL

#include "glad_i.h"
#include "sdl_gl.h"

#include "vertex.h"

void load_gl_buffers_tex(TexVertex *vertices, uint64_t vertices_amt, GLuint *vbo, GLuint *vao);

void load_gl_buffers_no_tex(Vertex *vertices, uint64_t vertices_amt, GLuint *vbo, GLuint *vao);

#endif
