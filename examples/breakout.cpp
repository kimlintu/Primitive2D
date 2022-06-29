#include "../src/includes/engine.h"
#include "../src/includes/shader_program.h"
#include "../src/includes/texture.h"

#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <math.h>

static P2DTexQuadEntity *paddle;

//static P2DColorEllipseEntity *ball;
static P2DTexQuadEntity *ball;

static P2DString str; 

void game_init() {
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
    paddle = p2d_tex_quad_entity_new(paddle_position, paddle_position, paddle_width, paddle_height, test_default_shaders, "../res/test.png");
    if(!paddle) {
        // TODO: error handling
        exit(-1);
    }

    // Ball 
    // TODO: Maybe the ball should be a quad as well, since we're going to use sprites anyway 
    //test_default_shaders[0].src_path = "../src/shaders/def_color_ellipse.vert";
    //test_default_shaders[1].src_path = "../src/shaders/def_color_ellipse.frag";
    test_default_shaders[0].src_path = NULL;
    test_default_shaders[1].src_path = NULL;
    Vector2 ball_position = { (800.0f / 2.0f), (600.0f / 2.0f) - (32.0f / 2.0f) };
    //float ball_color[3] = { 0.4f, 0.64f, 0.9f };
    ball = p2d_tex_quad_entity_new(ball_position, ball_position, 32, 32, test_default_shaders, "../res/ball.png");
    if(!ball) {
        // TODO: error handling
        exit(-1);
    } else {
        ball->velocity.x = 6.0f;
        ball->velocity.y = 6.0f;
    }

    Vector2 str_pos = { 2.0f, 10.0f };
    str = p2d_put_string("ABC DEF", str_pos); 
}

bool p2d_aabb_collision(Vector2 pos_a, float width_a, float height_a, Vector2 pos_b, float width_b, float height_b) {
    if((pos_a.x <= (pos_b.x + width_b)) && ((pos_a.x + width_a) >= pos_b.x) &&
            (pos_a.y <= (pos_b.y + height_b)) && ((pos_a.y + height_a) >= pos_b.y)) {
        return true;
    }
    return false;
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

    ball->position.x += ball->velocity.x;
    ball->position.y += ball->velocity.y;
    if(p2d_aabb_collision(ball->position, ball->width, ball->height, paddle->position, paddle->width, paddle->height)) {
        // If the ball was above the paddle before it collided
        if((ball->position.y + ball->height) >= (paddle->position.y + (paddle->height >> 2))) {
            float paddle_velocity_x = fabs(paddle->velocity.x);
            if(ball->position.x > (paddle->position.x + (paddle->width >> 1))) {
                ball->position.x = paddle->position.x + paddle->width;
                if(paddle_velocity_x) {
                    ball->velocity.x = paddle_velocity_x;
                } else {
                    ball->velocity.x = 15.0f;
                }
            } else {
                ball->position.x = paddle->position.x - ball->width;
                if(paddle_velocity_x) {
                    ball->velocity.x = -paddle_velocity_x;
                } else {
                    ball->velocity.x = -15.0f;
                }
            }
        } else {
            ball->position.y = paddle->position.y - ball->height;
        }

        if(ball->velocity.y > 0) {
            ball->velocity.y = -ball->velocity.y;
        }
    } else {
        if(((ball->position.y + ball->height) >= 600.0f) || 
                ((ball->position.y) < 0.0f)) {
            ball->velocity.y = -ball->velocity.y;
        }

        if(((ball->position.x + ball->width) >= 800.0f) ||
                ((ball->position.x) <= 0.0f)) {
            ball->velocity.x = -ball->velocity.x;
        }
    }
}

void game_render(float dt) {
    p2d_quad_render(paddle, dt);
    p2d_quad_render(ball, dt);
    p2d_string_render(&str);
}
