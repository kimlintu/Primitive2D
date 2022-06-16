#include "engine.h"
#include <stdio.h>

// TODO: load textures
P2DQuadModel p2d_quad_model_new(uint32_t width, uint32_t height) {
    P2DQuadModel model = {};

    model.vertices[0].pos[0] = 0.0f;
    model.vertices[0].pos[1] = 1.0f;

    model.vertices[1].pos[0] = 1.0f;
    model.vertices[1].pos[1] = 1.0f;

    model.vertices[2].pos[0] = 1.0f;
    model.vertices[2].pos[1] = 0.0f;

    model.vertices[3].pos[0] = 0.0f;
    model.vertices[3].pos[1] = 0.0f;

    model.vertices[4].pos[0] = 0.0f;
    model.vertices[4].pos[1] = 1.0f;

    model.vertices[5].pos[0] = 1.0f;
    model.vertices[5].pos[1] = 0.0f;

    model.width = width;
    model.height = height;

    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    GLuint vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    
    glBufferData(GL_ARRAY_BUFFER, sizeof(model.vertices), model.vertices, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void *)0); // position

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void *)(2 * sizeof(float))); // texture coords

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    model.vao = vao;
    model.vbo = vbo;

    return model;
}

P2DQuadEntity p2d_quad_entity_new(float position[2], float rotation[2], P2DQuadModel *model, GLuint shader_program_id) {
    P2DQuadEntity entity = {};

    entity.model = model;
    entity.shader_program_id = shader_program_id;
    
    return entity;
}

void p2d_quad_render(P2DQuadEntity *quad) {
    glBindVertexArray(quad->model->vao);
    glUseProgram(quad->shader_program_id);

    glDrawArrays(GL_TRIANGLES, 0, 6);

    glUseProgram(0);
    glBindVertexArray(0);
}

// Thanks to https://www.youtube.com/watch?v=RgxR6akghe8&t=803s
bool key_pressed_this_frame(KeyboardKey key) {
    bool was_pressed = (key.is_down && key.half_transition_count == 1) || (key.half_transition_count > 1);

    return was_pressed;
}

bool key_released_this_frame(KeyboardKey key) {
    bool was_released = (!key.is_down && key.half_transition_count == 1) || (key.half_transition_count > 1);

    return was_released;
}

bool key_is_down(KeyboardKey key) {
    return key.is_down;
}
