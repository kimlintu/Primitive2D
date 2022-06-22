#include "../src/includes/engine.h"
#include "../src/includes/shader_program.h"
#include "../src/includes/texture.h"

#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>

static P2DQuadEntity *paddle;

static P2DEllipseEntity *ball;

Texture test;

void game_init() {
    test = load_texture("../res/test.png");
    P2D_init();

    // TODO: Here the user should be able to pass the initial window size and
    // additional parameters

    // If we specify the source path for the shaders the engine should use some default shaders
    Shader test_vertex_shader = { GL_VERTEX_SHADER, NULL };
    Shader test_fragment_shader = { GL_FRAGMENT_SHADER, NULL };
    Shader test_default_shaders[2] = { test_vertex_shader, test_fragment_shader };

    // Paddle
    float paddle_width = 64;
    float paddle_height = 32;
    Vector2 paddle_position; 
    paddle_position.x = (800.0f / 2.0f) - (paddle_width / 2.0f);
    paddle_position.y = (600.0f - paddle_height - 16.0f);
    paddle = p2d_quad_entity_new(paddle_position, paddle_position, paddle_width, paddle_height, test_default_shaders);
    if(!paddle) {
        // TODO: error handling
    }

    // Ball 
    // TODO: Maybe the ball should be a quad as well, since we're going to use sprites anyway 
    test_default_shaders[0].src_path = NULL;
    test_default_shaders[1].src_path = NULL;
    float ball_height = 32;
    Vector2 ball_position = { (800.0f / 2.0f), (600.0f / 2.0f) - (ball_height / 2.0f) };
    ball = p2d_ellipse_entity_new(ball_position, ball_position, 16, 16, test_default_shaders);
    if(!ball) {
        // TODO: error handling
    } else {
        ball->velocity.x = 2.0f;
        ball->velocity.y = 2.0f;
    }
    glBindTexture(GL_TEXTURE_2D, test.id);
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
    paddle->position.x += paddle->velocity.x;
    paddle->position.y += paddle->velocity.y;

    paddle->velocity.x = 0.0f;
    paddle->velocity.y = 0.0f;

    float speed = 20.0f;
    if(key_is_down(keyboard_state->keys[KEY_A])) {
        paddle->velocity.x -= speed;
    }

    if(key_is_down(keyboard_state->keys[KEY_D])) {
        paddle->velocity.x += speed;
    }

    if(key_is_down(keyboard_state->keys[KEY_S])) {
        paddle->velocity.y += speed;
    }

    if(key_is_down(keyboard_state->keys[KEY_W])) {
        paddle->velocity.y -= speed;
    }

    // Ball movement
    ball->position.x += ball->velocity.x;
    ball->position.y += ball->velocity.y;

    if(((ball->position.y + ball->height) >= 600.0f) || 
            ((ball->position.y - ball->height) < 0.0f)) {
        ball->velocity.y = -ball->velocity.y;
    }

    if(((ball->position.x + ball->width) >= 800.0f) ||
            ((ball->position.x - ball->width) <= 0.0f)) {
        ball->velocity.x = -ball->velocity.x;
    }
}

void game_render(float dt) {
    p2d_quad_render(paddle, dt);
    p2d_ellipse_render(ball, dt);
}
