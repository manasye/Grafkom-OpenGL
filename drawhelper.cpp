#include "drawhelper.hpp"
#include <GL/gl.h>
#include "filereader.cpp"

DrawHelper::DrawHelper() {
    // do nothing
}

void DrawHelper::add(Polygon polygon) {
    poly.push_back(polygon);
    // Triggers the vector to allocate more space
    vao.push_back(0);
    vbo.push_back(0);
    ebo.push_back(0);
    // Generate buffers
    glGenVertexArrays(1, &(vao[vao.size() - 1]));
    glGenBuffers(1, &(vbo[vbo.size() - 1]));
    glGenBuffers(1, &(ebo[ebo.size() - 1]));
    // Binding VAO, to allow binding to VBO & EBO
    glBindVertexArray(vao[vao.size() - 1]);
    // Associating VBO & EBO to the buffer
    glBindBuffer(GL_ARRAY_BUFFER, vbo[vbo.size() - 1]);
    glBufferData(GL_ARRAY_BUFFER, poly[poly.size() - 1].numOfVertex * (sizeof(float)), poly[poly.size() - 1].vertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo[ebo.size() - 1]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, poly[poly.size() - 1].numOfIndex * (sizeof(int)), poly[poly.size() - 1].indices, GL_STATIC_DRAW);
    // Positions
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);
    // Colors
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    // De-bind to allow other VAO/VBO/EBO to be allocated
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void DrawHelper::add(std::vector<Polygon> * poly) {
    for (std::vector<Polygon>::iterator ptr = poly->begin(); ptr != poly->end(); ++ptr) {
        this->add(*ptr);
    }
}

void DrawHelper::addFromFile(char * filename) {
    std::vector<Polygon> * poly = readPolygon(filename); 
    this->add(poly);
}

void DrawHelper::draw(int i) {
    glBindVertexArray(vao[i]);
    glDrawElements(GL_TRIANGLES, poly[i].numOfIndex, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

void DrawHelper::drawAll() {
    for (int i = 0; i < poly.size(); i++) {
        this->draw(i);
    }
}

void DrawHelper::remove(int i) {
    ebo.erase(ebo.begin() + i);
    vbo.erase(vbo.begin() + i);
    vao.erase(vao.begin() + i);
    poly.erase(poly.begin() + i);
}