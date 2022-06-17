#ifndef SHADER_PROGRAM_H
#define SHADER_PROGRAM_H

#include "glad_i.h"
#include "sdl_gl.h"

#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <glm/mat4x4.hpp>

struct Shader {
    GLenum type;
    char *src_path;
};

struct ShaderProgram {
    GLuint id;
};

int create_shader_program(struct Shader *shaders, uint32_t n, struct ShaderProgram *program);

void shader_program_load_mat4(GLuint program_id, const GLchar *uniform_name, glm::mat4 matrix);

#endif
