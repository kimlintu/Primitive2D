#include "../src/includes/engine.h"
#include "../src/includes/shader_program.h"
#include "../src/includes/texture.h"

#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <math.h>

#define WINDOW_WIDTH 1024
#define WINDOW_HEIGHT 768

#define FRAME_BUFFER_WIDTH 640
#define FRAME_BUFFER_HEIGHT 480 

static P2DTexQuadEntity *paddle;

//static P2DColorEllipseEntity *ball;
static P2DTexQuadEntity *ball;

static uint8_t level[4][5] = {
    { 1, 0, 1, 0, 1 },
    { 0, 1, 1, 1, 0 },
    { 0, 1, 1, 1, 0 },
    { 1, 0, 1, 0, 1 },
};
static P2DTexQuadEntity **bricks;

static P2DString str; 

uint32_t n_bricks = 0;

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
    paddle_position.x = (FRAME_BUFFER_WIDTH / 2.0f) - (paddle_width / 2.0f);
    paddle_position.y = (FRAME_BUFFER_HEIGHT - paddle_height - 16.0f);
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
    Vector2 ball_position = { (FRAME_BUFFER_WIDTH / 2.0f), (FRAME_BUFFER_HEIGHT / 2.0f) - (32.0f / 2.0f) };
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

    // Count bricks
    for(int i = 0; i < 4; i++) {
        for(int j = 0; j < 5; j++) {
            if(level[i][j]) {
                n_bricks++;
            }
        }
    }

    // Allocate bricks
    bricks = (P2DTexQuadEntity **)malloc(sizeof(P2DTexQuadEntity *) * n_bricks);
    int brick_i = 0;
    for(int i = 0; i < 4; i++) {
        for(int j = 0; j < 5; j++) {
            if(level[i][j]) {
                Vector2 brick_pos = { 4.0f + (j * (32.0f + 8.0f)), i * (32.0f + 4.0f) };
                bricks[brick_i++] = p2d_tex_quad_entity_new(brick_pos, { 0.0f, 0.0f }, 32, 32, test_default_shaders, "../res/ball.png");
            }
        }
    }

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

    // Paddle - Ball collision
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
        if(((ball->position.y) < 0.0f)) {
            ball->velocity.y = -ball->velocity.y;
        } else if((ball->position.y + ball->height) >= FRAME_BUFFER_HEIGHT) {
            ball->position.y = 0; 
        } else if(((ball->position.x + ball->width) >= FRAME_BUFFER_WIDTH) ||
                ((ball->position.x) <= 0.0f)) {
            ball->velocity.x = -ball->velocity.x;
        }
    }
}

void game_render(float dt) {
    p2d_quad_render(paddle, dt);
    p2d_quad_render(ball, dt);
    p2d_string_render(&str);

    for(int i = 0; i < n_bricks; i++) {
        p2d_quad_render(bricks[i], dt);
    }
}
