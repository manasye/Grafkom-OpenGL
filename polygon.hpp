#ifndef POLYGON_HPP
#define POLYGON_HPP

struct Polygon {
    float* vertices;
    unsigned int* indices;
    int numOfVertex;
    int numOfIndex;
    unsigned int texture;
};

#endif
