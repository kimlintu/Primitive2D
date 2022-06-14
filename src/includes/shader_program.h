#ifndef SHADER_PROGRAM_H
#define SHADER_PROGRAM_H

#include <glad/glad.h>
#include <SDL_opengl.h>

struct Shader {
    GLenum type;
    char *src_path;
};

struct ShaderProgram {
    GLuint id;
};

int create_shader_program(struct Shader *shaders, uint32_t n, struct ShaderProgram *program);

#endif
