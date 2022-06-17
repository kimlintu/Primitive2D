#include "../src/engine.h"
#include "../src/includes/shader_program.h"

static P2DQuadModel paddle_model;
static P2DQuadEntity paddle;

void game_init() {
    paddle_model = p2d_quad_model_new(32, 32);

    float position[2] = {};
    // load shaders
    Shader test_vertex_shader = { GL_VERTEX_SHADER, "../src/shaders/test.vert" };
    Shader test_fragment_shader = { GL_FRAGMENT_SHADER, "../src/shaders/test.frag" };
    Shader test_shaders[2] = { test_vertex_shader, test_fragment_shader };

    ShaderProgram test_shader_program;
    if(!create_shader_program(test_shaders, 2, &test_shader_program)) {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "ERROR [Could not create shader program]");
    }

    paddle = p2d_quad_entity_new(position, position, &paddle_model, test_shader_program.id);
}

void game_update(KeyboardState *keyboard_state) {
    paddle.position[0] += paddle.velocity[0];
    paddle.position[1] += paddle.velocity[1];

    paddle.velocity[0] = 0.0f;
    paddle.velocity[1] = 0.0f;

    float speed = 0.1f;
    if(key_is_down(keyboard_state->keys[KEY_A])) {
        paddle.velocity[0] = -speed;
    }

    if(key_is_down(keyboard_state->keys[KEY_D])) {
        paddle.velocity[0] = speed;
    }

    if(key_is_down(keyboard_state->keys[KEY_S])) {
        paddle.velocity[1] = -speed;
    }

    if(key_is_down(keyboard_state->keys[KEY_W])) {
        paddle.velocity[1] = speed;
    }
}

void game_render(float dt) {
    p2d_quad_render(&paddle, dt);
}
