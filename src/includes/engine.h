// TODO: move this file
#ifndef ENGINE_H
#define ENGINE_H

#include "includes/glad_i.h"
#include "includes/sdl_gl.h"

struct Vector2 {
    float x;
    float y;
};

struct Vertex {
    Vector2 pos; // TODO: replace with glm::vec2?
    Vector2 tex;
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
    Vector2 position; // TODO: replace with glm::vec2?
    Vector2 velocity;
    Vector2 rotation;
    GLuint shader_program_id;
};

P2DQuadEntity p2d_quad_entity_new(Vector2 position, Vector2 rotation, P2DQuadModel *model, GLuint shader_program_id); 
void p2d_quad_render(P2DQuadEntity *quad, float dt);

enum KeyboardKeyID {
    KEY_W,
    KEY_A,
    KEY_S,
    KEY_D,

    MAX_KEY_AMT, // Since the first enum member is set to 0 and the following members increase incrementally, we can use this member to figure out the max amount of possible keys
};

struct KeyboardKey {
    uint32_t half_transition_count;
    bool is_down;
};

// TODO: This could be a more generalized 'input state' to allow switching between different input types (e.g. keyboard and controller)
struct KeyboardState {
    KeyboardKey keys[MAX_KEY_AMT];
};

bool key_pressed_this_frame(KeyboardKey key);
bool key_released_this_frame(KeyboardKey key);
bool key_is_down(KeyboardKey key);

#endif
