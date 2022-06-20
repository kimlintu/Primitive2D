#include "includes/engine.h"
#include "includes/shader_program.h"

#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/matrix.hpp>
#include <glm/ext/matrix_transform.hpp>

// TODO: load textures
P2DQuadModel p2d_quad_model_new(uint32_t width, uint32_t height) {
    P2DQuadModel model = {};

    model.vertices[0].pos.x = 0.0f;
    model.vertices[0].pos.y = 1.0f;

    model.vertices[1].pos.x = 1.0f;
    model.vertices[1].pos.y = 1.0f;

    model.vertices[2].pos.x = 1.0f;
    model.vertices[2].pos.y = 0.0f;

    model.vertices[3].pos.x = 0.0f;
    model.vertices[3].pos.y = 0.0f;

    model.vertices[4].pos.x = 0.0f;
    model.vertices[4].pos.y = 1.0f;

    model.vertices[5].pos.x = 1.0f;
    model.vertices[5].pos.y = 0.0f;

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

P2DQuadEntity p2d_quad_entity_new(Vector2 position, Vector2 rotation, P2DQuadModel *model, GLuint shader_program_id) {
    P2DQuadEntity entity = {};

    entity.position.x = position.x;
    entity.position.y = position.y;
    entity.rotation.x = rotation.x;
    entity.rotation.y = rotation.y;
    entity.model = model;
    entity.shader_program_id = shader_program_id;
    
    return entity;
}

void p2d_quad_render(P2DQuadEntity *quad, float dt) {
    glUseProgram(quad->shader_program_id);
    glm::mat4 model = glm::mat4(1.0f);

    float pos_x = quad->position.x + (quad->velocity.x * dt);
    float pos_y = quad->position.y + (quad->velocity.y * dt);

    model = glm::translate(model, glm::vec3(pos_x, pos_y, 1.0f));
    model = glm::scale(model, glm::vec3(quad->model->width, quad->model->height, 1.0f));

    shader_program_load_mat4(quad->shader_program_id, "model", model);

    glBindVertexArray(quad->model->vao);

    glDrawArrays(GL_TRIANGLES, 0, 6);

    glUseProgram(0);
    glBindVertexArray(0);
}

P2DCircleModel p2d_circle_model_new(uint32_t width, uint32_t height) {
    P2DCircleModel model = {};

    float PI = 3.14159265359;
    float pos_x_45 = cos(PI / 4.0f); 
    float pos_y_45 = sin(PI / 4.0f); 

    // Center
    model.vertices[0].pos.x = 0.0f;
    model.vertices[0].pos.y = 0.0f;

    // Top
    model.vertices[1].pos.x = 0.0f;
    model.vertices[1].pos.y = 1.0f;

    model.vertices[2].pos.x = pos_x_45;
    model.vertices[2].pos.y = pos_y_45;

    // Right
    model.vertices[3].pos.x = 1.0f;
    model.vertices[3].pos.y = 0.0f;

    model.vertices[4].pos.x = pos_x_45;
    model.vertices[4].pos.y = -pos_y_45;

    // Bottom
    model.vertices[5].pos.x = 0.0f;
    model.vertices[5].pos.y = -1.0f;

    model.vertices[6].pos.x = -pos_x_45;
    model.vertices[6].pos.y = -pos_y_45;

    // Left
    model.vertices[7].pos.x = -1.0f;
    model.vertices[7].pos.y = 0.0f;

    model.vertices[8].pos.x = -pos_x_45;
    model.vertices[8].pos.y = pos_y_45;

    // Top
    model.vertices[9].pos.x = 0.0f;
    model.vertices[9].pos.y = 1.0f;

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

P2DCircleEntity p2d_circle_entity_new(Vector2 position, Vector2 rotation, P2DCircleModel *model, GLuint shader_program_id) {
    P2DCircleEntity entity = {};

    entity.position.x = position.x;
    entity.position.y = position.y;
    entity.rotation.x = rotation.x;
    entity.rotation.y = rotation.y;
    entity.velocity.x = 2.0f;
    entity.velocity.y = 5.0f;
    entity.model = model;
    entity.shader_program_id = shader_program_id;
    
    return entity;
}

void p2d_circle_render(P2DCircleEntity *circle, float dt) {
    glUseProgram(circle->shader_program_id);
    glm::mat4 model = glm::mat4(1.0f);

    float pos_x = circle->position.x + (circle->velocity.x * dt);
    float pos_y = circle->position.y + (circle->velocity.y * dt);

    model = glm::translate(model, glm::vec3(pos_x, pos_y, 1.0f));
    model = glm::scale(model, glm::vec3(circle->model->width, circle->model->height, 1.0f));

    shader_program_load_mat4(circle->shader_program_id, "model", model);

    glBindVertexArray(circle->model->vao);

    glDrawArrays(GL_TRIANGLE_FAN, 0, 10);

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
