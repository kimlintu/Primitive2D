#ifndef GL_UTIL
#define GL_UTIL

#include "includes/glad_i.h"
#include "includes/sdl_gl.h"

#include "includes/vertex.h"

// TODO: rename?
void load_gl_buffers_v(Vertex *vertices, uint64_t vertices_amt, GLuint *vbo, GLuint *vao);
void load_gl_buffers_pv(PosVertex *vertices, uint64_t vertices_amt, GLuint *vbo, GLuint *vao);

#endif
