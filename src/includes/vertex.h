#ifndef P2D_VERTEX_H
#define P2D_VERTEX_H

struct Vector2 {
    float x;
    float y;
};

struct Vertex {
    Vector2 pos; 
}; 

struct TexVertex {
    Vector2 pos; 
    Vector2 tex;
}; 

#endif
