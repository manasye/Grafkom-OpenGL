#ifndef POLYGON_HPP
#define POLYGON_HPP

#include <GL/gl.h>

struct Polygon {
    float* vertices;
    unsigned int* indices;
    int numOfVertex;
    int numOfIndex;
    GLuint texture;
};

#endif
