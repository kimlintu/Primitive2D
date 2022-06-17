#include "../src/includes/engine.h"
#include "../src/includes/shader_program.h"

#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>

static P2DQuadModel paddle_model;
static P2DQuadEntity paddle;

void game_init() {
    // TODO: Here the user should be able to pass the initial window size and
    // additional parameters

    // load shaders
    Shader test_vertex_shader = { GL_VERTEX_SHADER, "../src/shaders/test.vert" };
    Shader test_fragment_shader = { GL_FRAGMENT_SHADER, "../src/shaders/test.frag" };
    Shader test_shaders[2] = { test_vertex_shader, test_fragment_shader };

    ShaderProgram test_shader_program;
    if(!create_shader_program(test_shaders, 2, &test_shader_program)) {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "ERROR [Could not create shader program]");
    }

    glm::mat4 projection = glm::ortho(0.0f, 800.0f, 600.0f, 0.0f, -1.0f, 1.0f);

    glUseProgram(test_shader_program.id);
    shader_program_load_mat4(test_shader_program.id, "projection", projection);
    glUseProgram(0);

    
    paddle_model = p2d_quad_model_new(64, 32);

    Vector2 paddle_position; 
    paddle_position.x = (800.0f / 2.0f) - (paddle_model.width / 2.0f);
    paddle_position.y = (600.0f - paddle_model.height - 16.0f);

    paddle = p2d_quad_entity_new(paddle_position, paddle_position, &paddle_model, test_shader_program.id);
}

void game_update(KeyboardState *keyboard_state) {
    paddle.position.x += paddle.velocity.x;
    paddle.position.y += paddle.velocity.y;

    paddle.velocity.x = 0.0f;
    paddle.velocity.y = 0.0f;

    float speed = 20.0f;
    if(key_is_down(keyboard_state->keys[KEY_A])) {
        paddle.velocity.x -= speed;
    }

    if(key_is_down(keyboard_state->keys[KEY_D])) {
        paddle.velocity.x += speed;
    }

    if(key_is_down(keyboard_state->keys[KEY_S])) {
        paddle.velocity.y += speed;
    }

    if(key_is_down(keyboard_state->keys[KEY_W])) {
        paddle.velocity.y -= speed;
    }
}

void game_render(float dt) {
    p2d_quad_render(&paddle, dt);
}
