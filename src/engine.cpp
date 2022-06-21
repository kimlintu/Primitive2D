#include "includes/engine.h"
#include "includes/shader_program.h"
#include "includes/gl_util.h"

#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/matrix.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/gtc/matrix_transform.hpp>

static float PI = 3.14159265359f;

#define P2D_MAX_QUAD_ENTITIES 256
static P2DQuadModel p2d_g_quad_model;
static P2DQuadEntity p2d_g_quad_entities[P2D_MAX_QUAD_ENTITIES] = {};
static uint8_t p2d_g_active_quad_entities[P2D_MAX_QUAD_ENTITIES] = {};

#define P2D_MAX_ELLIPSE_ENTITIES 256
static P2DEllipseModel p2d_g_ellipse_model;
static P2DEllipseEntity p2d_g_ellipse_entities[P2D_MAX_ELLIPSE_ENTITIES] = {};
static uint8_t p2d_g_active_ellipse_entities[P2D_MAX_ELLIPSE_ENTITIES] = {};

// TODO: load textures
void p2d_quad_model_init() {
    p2d_g_quad_model.vertices[0].pos.x = 0.0f;
    p2d_g_quad_model.vertices[0].pos.y = 1.0f;

    p2d_g_quad_model.vertices[1].pos.x = 1.0f;
    p2d_g_quad_model.vertices[1].pos.y = 1.0f;

    p2d_g_quad_model.vertices[2].pos.x = 1.0f;
    p2d_g_quad_model.vertices[2].pos.y = 0.0f;

    p2d_g_quad_model.vertices[3].pos.x = 0.0f;
    p2d_g_quad_model.vertices[3].pos.y = 0.0f;

    p2d_g_quad_model.vertices[4].pos.x = 0.0f;
    p2d_g_quad_model.vertices[4].pos.y = 1.0f;

    p2d_g_quad_model.vertices[5].pos.x = 1.0f;
    p2d_g_quad_model.vertices[5].pos.y = 0.0f;

    load_gl_buffers(p2d_g_quad_model.vertices, 6, &p2d_g_quad_model.vbo, &p2d_g_quad_model.vao);
}

int p2d_load_entity_shaders(ShaderProgram *shader_program, Shader shaders[2]) {
    // If user did not specify their own shaders, we just load some default ones
    if(!shaders[0].src_path) {
        shaders[0].src_path = "../src/shaders/test.vert";
        // TODO: Maybe the path should be reset to NULL before returning
    }

    if(!shaders[1].src_path) {
        shaders[1].src_path = "../src/shaders/test.frag";
        // TODO: Maybe the path should be reset to NULL before returning
    }

    if(!create_shader_program(shaders, 2, shader_program)) {
        // TODO: error handling
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "ERROR [Could not create shader program]");

        return 0;
    }
    glm::mat4 projection = glm::ortho(0.0f, 800.0f, 600.0f, 0.0f, -1.0f, 1.0f);
    glUseProgram(shader_program->id);
    shader_program_load_mat4(shader_program->id, "projection", projection);
    glUseProgram(0);
}

P2DQuadEntity *p2d_quad_entity_new(Vector2 position, Vector2 rotation, uint32_t width, uint32_t height, Shader shaders[2]) {
    uint8_t entity_id = 0;
    // Find available entity
    while(p2d_g_active_quad_entities[entity_id] && (entity_id < P2D_MAX_QUAD_ENTITIES)) {
        entity_id++;
    }

    if(entity_id == P2D_MAX_QUAD_ENTITIES) {
        // TODO: error handling
        printf("No quad entities available\n");

        return NULL;
    }
    p2d_g_active_quad_entities[entity_id] = 1;

    P2DQuadEntity *entity = (p2d_g_quad_entities + entity_id);
    if(!p2d_load_entity_shaders(&entity->shader_program, shaders)) {
        // TODO: error handling

        return NULL;
    }

    entity->position.x = position.x;
    entity->position.y = position.y;
    entity->rotation.x = rotation.x;
    entity->rotation.y = rotation.y;
    entity->width = width;
    entity->height = height;
    entity->model = &p2d_g_quad_model;

    return entity;
}

void p2d_quad_render(P2DQuadEntity *quad, float dt) {
    glUseProgram(quad->shader_program.id);
    glm::mat4 model = glm::mat4(1.0f);

    float pos_x = quad->position.x + (quad->velocity.x * dt);
    float pos_y = quad->position.y + (quad->velocity.y * dt);

    model = glm::translate(model, glm::vec3(pos_x, pos_y, 1.0f));
    model = glm::scale(model, glm::vec3(quad->width, quad->height, 1.0f));

    shader_program_load_mat4(quad->shader_program.id, "model", model);

    glBindVertexArray(quad->model->vao);

    glDrawArrays(GL_TRIANGLES, 0, 6);

    glUseProgram(0);
    glBindVertexArray(0);
}

void p2d_ellipse_model_init() {
    float pos_x_45 = cos(PI / 4.0f); 
    float pos_y_45 = sin(PI / 4.0f); 

    // Center
    p2d_g_ellipse_model.vertices[0].pos.x = 0.0f;
    p2d_g_ellipse_model.vertices[0].pos.y = 0.0f;

    // Top
    p2d_g_ellipse_model.vertices[1].pos.x = 0.0f;
    p2d_g_ellipse_model.vertices[1].pos.y = 1.0f;

    p2d_g_ellipse_model.vertices[2].pos.x = pos_x_45;
    p2d_g_ellipse_model.vertices[2].pos.y = pos_y_45;

    // Right
    p2d_g_ellipse_model.vertices[3].pos.x = 1.0f;
    p2d_g_ellipse_model.vertices[3].pos.y = 0.0f;

    p2d_g_ellipse_model.vertices[4].pos.x = pos_x_45;
    p2d_g_ellipse_model.vertices[4].pos.y = -pos_y_45;

    // Bottom
    p2d_g_ellipse_model.vertices[5].pos.x = 0.0f;
    p2d_g_ellipse_model.vertices[5].pos.y = -1.0f;

    p2d_g_ellipse_model.vertices[6].pos.x = -pos_x_45;
    p2d_g_ellipse_model.vertices[6].pos.y = -pos_y_45;

    // Left
    p2d_g_ellipse_model.vertices[7].pos.x = -1.0f;
    p2d_g_ellipse_model.vertices[7].pos.y = 0.0f;

    p2d_g_ellipse_model.vertices[8].pos.x = -pos_x_45;
    p2d_g_ellipse_model.vertices[8].pos.y = pos_y_45;

    // Top
    p2d_g_ellipse_model.vertices[9].pos.x = 0.0f;
    p2d_g_ellipse_model.vertices[9].pos.y = 1.0f;

    load_gl_buffers(p2d_g_ellipse_model.vertices, 10, &p2d_g_ellipse_model.vbo, &p2d_g_ellipse_model.vao);
}

P2DEllipseEntity *p2d_ellipse_entity_new(Vector2 position, Vector2 rotation, uint32_t width, uint32_t height, Shader shaders[2]) {
    uint8_t entity_id = 0;
    // Find available entity
    while(p2d_g_active_ellipse_entities[entity_id] && (entity_id < P2D_MAX_ELLIPSE_ENTITIES)) {
        entity_id++;
    }

    if(entity_id == P2D_MAX_ELLIPSE_ENTITIES) {
        // TODO: error handling
        printf("No ellipse entities available\n");

        return NULL;
    }
    p2d_g_active_ellipse_entities[entity_id] = 1;

    P2DEllipseEntity *entity = (p2d_g_ellipse_entities + entity_id);
    if(!p2d_load_entity_shaders(&entity->shader_program, shaders)) {
        // TODO: error handling

        return NULL;
    }

    entity->position.x = position.x;
    entity->position.y = position.y;
    entity->rotation.x = rotation.x;
    entity->rotation.y = rotation.y;
    entity->width = width;
    entity->height = height;
    entity->model = &p2d_g_ellipse_model;

    return entity;
}

void p2d_ellipse_render(P2DEllipseEntity *ellipse, float dt) {
    glUseProgram(ellipse->shader_program.id);
    glm::mat4 model = glm::mat4(1.0f);

    float pos_x = ellipse->position.x + (ellipse->velocity.x * dt);
    float pos_y = ellipse->position.y + (ellipse->velocity.y * dt);

    model = glm::translate(model, glm::vec3(pos_x, pos_y, 1.0f));
    model = glm::scale(model, glm::vec3(ellipse->width, ellipse->height, 1.0f));

    shader_program_load_mat4(ellipse->shader_program.id, "model", model);

    glBindVertexArray(ellipse->model->vao);

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

void P2D_init() {
    p2d_quad_model_init();
    p2d_ellipse_model_init();
}
