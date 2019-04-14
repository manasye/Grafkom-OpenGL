#ifndef DRAWHELPER_HPP
#define DRAWHELPER_HPP

#include <vector>
#include <iostream>
#include <fstream>
#include "polygon.hpp"

class DrawHelper {
    public:
        DrawHelper();
        // CALL THESE FUNCTIONS ONLY AFTER INITIALIZING GL
        void add(Polygon polygon);
        void add(std::vector<Polygon> * polyVec);
        void loadFromFile(char * filename);
        void loadTexture(char * filename);
        void draw(int i);
        void drawAll();
        void remove(int i);

    private:
        std::vector<Polygon> poly;
        std::vector<GLuint> textures;
        std::vector<GLuint> vao;
        std::vector<GLuint> vbo;
        std::vector<GLuint> ebo;

};

#endif