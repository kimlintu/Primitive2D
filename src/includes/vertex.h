#ifndef P2D_VERTEX_H
#define P2D_VERTEX_H

struct Vector2 {
    float x;
    float y;
};

// The quad (for example) will have the same texture coordinates as position coordinates,
// so we do not have to save both
struct PosVertex {
    Vector2 pos; 
};

struct Vertex {
    Vector2 pos; 
    Vector2 tex;
}; 

#endif
