#include "includes/shader_program.h"

#define INIT_BFR_SIZE 500

int create_shader_program(struct Shader *shaders, uint32_t n, struct ShaderProgram *program) {
    GLuint program_id = glCreateProgram();
    if(!program_id) {
        fprintf(stderr, "Could not create program.\n");
        return 0;
    }

    if(n > 128) {
        fprintf(stderr, "Too many shaders. Max is 128.\n");
        // TODO: What is a reasonable max value?
        return 0;
    }
    
    GLuint *attached_shaders = (GLuint *)malloc(n * sizeof(GLuint));

    for(int n_shader = 0; n_shader < n; n_shader++) {
        struct Shader *sh = &shaders[n_shader];

        FILE *src = fopen(sh->src_path, "r");
        if(src == NULL) {
            fprintf(stderr, "Could not open shader source file (%s)!\nError:%s\n", shaders[n_shader].src_path, strerror(errno));

            free(attached_shaders);
            return 0;
        }

        uint32_t bfr_size = INIT_BFR_SIZE;
        char *text = (char *)malloc(bfr_size * sizeof(char));
        for(int i = 0; i < bfr_size; i++) {
            text[i] = (char)0;
        }

        uint32_t chars_n = 0;
        char c;
        while((c = fgetc(src)) != EOF) {
            text[chars_n] = c;
            chars_n++;

            if(chars_n >= bfr_size) {
                printf("Error: buffer too small\n");

                free(text);
                free(attached_shaders);
                return 0;
            }
        }
        const GLint len = (const GLint) strlen(text);

        GLuint sh_id = glCreateShader(sh->type);
        glShaderSource(sh_id, 1, (const GLchar **)&text, &len);
        glCompileShader(sh_id);

        free(text);

        GLint status;
        glGetShaderiv(sh_id, GL_COMPILE_STATUS, &status);
        if(status == GL_FALSE)
        {
            char info[512];
            glGetShaderInfoLog(sh_id, 512, NULL, info);
            fprintf(stderr, "Could not compile shader.\nError: %s\n", info);

            free(attached_shaders);
            return 0;
        }

        if(fclose(src) != 0) {
            fprintf(stderr, "Could not close shader source file\nError: %s\n", strerror(errno));

            free(attached_shaders);
            return 0;
        }

        glAttachShader(program_id, sh_id);
        attached_shaders[n_shader] = sh_id;
    }

    glLinkProgram(program_id);

    for(int n_shader = 0; n_shader < n; n_shader++) {
        glDeleteShader(attached_shaders[n_shader]);
    }

    program->id = program_id;

    free(attached_shaders);
    return 1;
}

#include <glm/gtc/type_ptr.hpp>
void shader_program_load_mat4(GLuint program_id, const GLchar *uniform_name, glm::mat4 matrix) {
    GLint location = glGetUniformLocation(program_id, uniform_name);
    glUniformMatrix4fv(location, (GLsizei)1, GL_FALSE, glm::value_ptr(matrix));
}
