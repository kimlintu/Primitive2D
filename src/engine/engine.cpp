#include "../includes/engine.h"
#include "../includes/shader_program.h"
#include "../includes/gl_util.h"

#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/matrix.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/gtc/matrix_transform.hpp>

static float PI = 3.14159265359f;

#define P2D_MAX_QUAD_ENTITIES 256
static P2DQuadModel p2d_g_quad_model;
static P2DTexQuadEntity p2d_g_quad_entities[P2D_MAX_QUAD_ENTITIES] = {};
static uint8_t p2d_g_active_quad_entities[P2D_MAX_QUAD_ENTITIES] = {};

#define P2D_MAX_ELLIPSE_ENTITIES 256
static P2DEllipseModel p2d_g_ellipse_model;
static P2DColorEllipseEntity p2d_g_ellipse_entities[P2D_MAX_ELLIPSE_ENTITIES] = {};
static uint8_t p2d_g_active_ellipse_entities[P2D_MAX_ELLIPSE_ENTITIES] = {};

void p2d_tex_quad_model_init() {
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

    // The texture vertices will be the same as the position vertices
    load_gl_buffers_no_tex(p2d_g_quad_model.vertices, 6, &p2d_g_quad_model.vbo, &p2d_g_quad_model.vao);
}

int p2d_load_entity_shaders(ShaderProgram *shader_program, Shader shaders[2]) {
    // If user did not specify their own shaders, we just load some default ones
    // TODO: This should be moved outside this function, since different entities will have different default shaders
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
    // TODO: The window dimensions should not be constants
    glm::mat4 projection = glm::ortho(0.0f, 640.0f, 480.0f, 0.0f, -1.0f, 1.0f);
    glUseProgram(shader_program->id);
    shader_program_load_mat4(shader_program->id, "projection", projection);
    glUseProgram(0);
}

P2DTexQuadEntity *p2d_tex_quad_entity_new(Vector2 position, Vector2 rotation, uint32_t width, uint32_t height,  Shader shaders[2], const char *texture_path) {
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

    P2DTexQuadEntity *entity = (p2d_g_quad_entities + entity_id);
    if(!p2d_load_entity_shaders(&entity->shader_program, shaders)) {
        // TODO: error handling

        return NULL;
    }

    entity->texture = load_texture(texture_path);
    if(!entity->texture.id) {
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

void p2d_quad_render(P2DTexQuadEntity *quad, float dt) {
    glBindTexture(GL_TEXTURE_2D, quad->texture.id);
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
    glBindTexture(GL_TEXTURE_2D, 0);
}

void p2d_col_ellipse_model_init() {
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

    load_gl_buffers_no_tex(p2d_g_ellipse_model.vertices, 10, &p2d_g_ellipse_model.vbo, &p2d_g_ellipse_model.vao);
}

P2DColorEllipseEntity *p2d_col_ellipse_entity_new(Vector2 position, Vector2 rotation, uint32_t width, uint32_t height, Shader shaders[2], float rgb_color[3]) {
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

    P2DColorEllipseEntity *entity = (p2d_g_ellipse_entities + entity_id);
    if(!p2d_load_entity_shaders(&entity->shader_program, shaders)) {
        // TODO: error handling

        return NULL;
    }

    // Load color uniform variable
    glUseProgram(entity->shader_program.id);
    glUniform1fv(glGetUniformLocation(entity->shader_program.id, "unfRGBColor"), 3, rgb_color);
    glUseProgram(0);

    entity->position.x = position.x;
    entity->position.y = position.y;
    entity->rotation.x = rotation.x;
    entity->rotation.y = rotation.y;
    entity->width = width;
    entity->height = height;
    entity->model = &p2d_g_ellipse_model;

    return entity;
}

void p2d_ellipse_render(P2DColorEllipseEntity *ellipse, float dt) {
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

Texture p2d_g_font_bitmap;
ShaderProgram p2d_g_font_shader_program;

void p2d_font_bitmap_init() {
    p2d_g_font_bitmap = load_texture("../res/font_bitmap.png");

    Shader shaders[2] = { { GL_VERTEX_SHADER, "../src/shaders/def_font.vert" }, { GL_FRAGMENT_SHADER, "../src/shaders/def_font.frag" } };
    if(!p2d_load_entity_shaders(&p2d_g_font_shader_program, shaders)) {
        // TODO: error handling
    }
}

P2DString p2d_put_string(const char *str, Vector2 str_pos) {
    P2DString new_str = {};

    size_t str_len = strnlen(str, 256);
    new_str.len = str_len;
    
    new_str.str = (P2DChar *)malloc(sizeof(P2DChar) * str_len);

    for(int i = 0; i < str_len; i++) {
        new_str.str[i].pos.x = str_pos.x + (i * 64.0f);
        new_str.str[i].pos.y = str_pos.y;

        // TODO: Check if letter, number, invalid character, uppercase lowercase, etc.
        new_str.str[i].tex_offset = (str[i] == ' ') ? 0 : (str[i] - 'A'); 
    } 

    return new_str;
}

void p2d_char_render(P2DChar *chr) {
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(chr->pos.x, chr->pos.y, 1.0f));
    model = glm::scale(model, glm::vec3(64.0f, 64.0f, 1.0f));

    shader_program_load_mat4(p2d_g_font_shader_program.id, "model", model);
    shader_program_load_float(p2d_g_font_shader_program.id, "unfTexOffset", chr->tex_offset);

    glDrawArrays(GL_TRIANGLES, 0, 6);
}

void p2d_string_render(P2DString *str) {
    glBindTexture(GL_TEXTURE_2D, p2d_g_font_bitmap.id);
    glUseProgram(p2d_g_font_shader_program.id);
    glm::mat4 model = glm::mat4(1.0f);

    glBindVertexArray(p2d_g_quad_model.vao);

    for(int i = 0; i < str->len; i++) {
        p2d_char_render(&str->str[i]);
    }

    glBindVertexArray(0);
    glUseProgram(0);
    glBindTexture(GL_TEXTURE_2D, 0);
}

bool p2d_aabb_collision(Vector2 pos_a, float width_a, float height_a, Vector2 pos_b, float width_b, float height_b) {
    if((pos_a.x <= (pos_b.x + width_b)) && ((pos_a.x + width_a) >= pos_b.x) &&
            (pos_a.y <= (pos_b.y + height_b)) && ((pos_a.y + height_a) >= pos_b.y)) {
        return true;
    }
    return false;
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
    p2d_font_bitmap_init();
    p2d_tex_quad_model_init();
    p2d_col_ellipse_model_init();
}
