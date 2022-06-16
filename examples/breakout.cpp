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
}

void game_render() {
    p2d_quad_render(&paddle);
}
