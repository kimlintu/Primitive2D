#include "includes/gl_util.h"

void load_gl_buffers(Vertex *vertices, uint64_t n, GLuint *vbo, GLuint *vao) {
    glGenVertexArrays(1, vao);
    glBindVertexArray(*vao);

    glGenBuffers(1, vbo);
    glBindBuffer(GL_ARRAY_BUFFER, *vbo);
    
    glBufferData(GL_ARRAY_BUFFER, sizeof(*vertices) * n, vertices, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void *)0); // position

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void *)(2 * sizeof(float))); // texture coords

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}
