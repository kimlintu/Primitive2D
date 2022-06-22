#ifndef ENGINE_H
#define ENGINE_H

#include "includes/glad_i.h"
#include "includes/sdl_gl.h"

#include "includes/shader_program.h"
#include "includes/texture.h"
#include "includes/vertex.h"

void P2D_init();

struct P2DQuadModel {
    PosVertex vertices[6];
    GLuint vao;
    GLuint vbo;
};

// TODO: Extract joint entity parameters (right now everything except model) to a struct
struct P2DTexQuadEntity {
    P2DQuadModel *model;
    Vector2 position; // TODO: replace with glm::vec2?
    Vector2 velocity;
    Vector2 rotation;
    uint32_t width; 
    uint32_t height;
    ShaderProgram shader_program;
    Texture texture;
};

P2DTexQuadEntity *p2d_tex_quad_entity_new(Vector2 position, Vector2 rotation, uint32_t width, uint32_t height,  Shader shaders[2], const char *texture_path);
void p2d_quad_render(P2DTexQuadEntity *quad, float dt);

struct P2DEllipseModel {
    PosVertex vertices[10];
    GLuint vao;
    GLuint vbo;
};

struct P2DColorEllipseEntity {
    P2DEllipseModel *model;
    Vector2 position; // TODO: replace with glm::vec2?
    Vector2 velocity;
    Vector2 rotation;
    uint32_t width; 
    uint32_t height;
    ShaderProgram shader_program;
    float color[3];
};

P2DColorEllipseEntity *p2d_col_ellipse_entity_new(Vector2 position, Vector2 rotation, uint32_t width, uint32_t height, Shader shaders[2], float rgb_color[3]);
void p2d_ellipse_render(P2DColorEllipseEntity *ellipse, float dt);

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
