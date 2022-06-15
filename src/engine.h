#ifndef ENGINE_H
#define ENGINE_H

#include "includes/glad_i.h"
#include "includes/sdl_gl.h"

struct Vertex {
    float pos[2]; // position coordinates
    float tex[2]; // texture coordinates
}; 

struct P2DQuadModel {
    Vertex vertices[6];
    uint32_t width;
    uint32_t height;
    GLuint vao;
    GLuint vbo;
};

struct P2DQuadModel p2d_quad_model_new(uint32_t width, uint32_t height);

struct P2DQuadEntity {
    P2DQuadModel *model;
    float position[2];
    float rotation[2];
    GLuint shader_program_id;
};

P2DQuadEntity p2d_quad_entity_new(float position[2], float rotation[2], P2DQuadModel *model, GLuint shader_program_id);
void p2d_quad_render(P2DQuadEntity *quad);

#endif
