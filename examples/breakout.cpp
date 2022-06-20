#include "../src/includes/engine.h"
#include "../src/includes/shader_program.h"

#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>

static P2DQuadModel paddle_model;
static P2DQuadEntity paddle;

static P2DCircleModel ball_model;
static P2DCircleEntity ball;

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

    // Paddle
    paddle_model = p2d_quad_model_new(64, 32);

    Vector2 paddle_position; 
    paddle_position.x = (800.0f / 2.0f) - (paddle_model.width / 2.0f);
    paddle_position.y = (600.0f - paddle_model.height - 16.0f);
    paddle = p2d_quad_entity_new(paddle_position, paddle_position, &paddle_model, test_shader_program.id);

    // Ball 
    // TODO: Maybe the ball should be a quad as well, since we're going to use sprites anyway 
    ball_model = p2d_circle_model_new(16.0f, 16.0f);

    Vector2 ball_position = { (800.0f / 2.0f), (600.0f / 2.0f) - (ball_model.height / 2.0f) };
    ball = p2d_circle_entity_new(ball_position, ball_position, &ball_model, test_shader_program.id);
}

void game_update(KeyboardState *keyboard_state) {
    // TODO: Switch between states
    // Example:
    // switch(game.state) {
    //      case MAIN_MENU:
    //          ...
    //      case INIT_GAME:
    //          load/create entities/maps
    //      case INGAME:
    //          ...
    //      case INGAME_MENU:
    //          ...
    // }

    // Paddle movement
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

    // Ball movement
    ball.position.x += ball.velocity.x;
    ball.position.y += ball.velocity.y;

    if(((ball.position.y + ball.model->height) >= 600.0f) || 
            ((ball.position.y - ball.model->height) < 0.0f)) {
        ball.velocity.y = -ball.velocity.y;
    }

    if(((ball.position.x + ball.model->width) >= 800.0f) ||
            ((ball.position.x - ball.model->width) <= 0.0f)) {
        ball.velocity.x = -ball.velocity.x;
    }
}

void game_render(float dt) {
    p2d_quad_render(&paddle, dt);
    p2d_circle_render(&ball, dt);
}
