#include "drawhelper.hpp"
#include <GL/gl.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string>
#include "lib/json.hpp"
#define STB_IMAGE_IMPLEMENTATION
#include "lib/stb_image.h"

using namespace std;
using namespace nlohmann;

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
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);
    // Colors
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    // Texture Position
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void *)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);
    // Normal
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void *)(8 * sizeof(float)));
    glEnableVertexAttribArray(3);

    // De-bind to allow other VAO/VBO/EBO to be allocated
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void DrawHelper::add(std::vector<Polygon> * poly) {
    for (std::vector<Polygon>::iterator ptr = poly->begin(); ptr != poly->end(); ++ptr) {
        this->add(*ptr);
    }
}

void DrawHelper::loadFromFile(const char * filename) {
    // Opening & converting JSON file to internal representation
    ifstream inputFile;
    json inputJson;

    inputFile.open(filename);
    inputFile >> inputJson;

    json textures = inputJson["textures"];
    json objects = inputJson["objects"];

    // If there are textures defined
    if (!textures.empty()) {
        for (int i = 0; i < textures.size(); i++) {
            string current = textures[i].get<string>();
            const char * filename = current.c_str();
            this->loadTexture(filename);
        }
    }

    // If there are objects defined
    if (!objects.empty()) {
        for (json::iterator it = objects.begin(); it != objects.end(); ++it) {
            Polygon temp;
            json points = (*it)["points"];
            json colors = (*it)["colors"];
            json indices = (*it)["indices"];
            json texCoord = (*it)["texPos"];
            json normals = (*it)["normals"];

            // Allocating array for vertex and indices
            temp.numOfVertex = 3 * points.size() + ((points.size() / 3) * 2);
            temp.vertices = (float *) malloc(temp.numOfVertex * sizeof(float));
            temp.numOfIndex = indices.size();
            temp.indices = (unsigned int *) malloc(temp.numOfIndex * sizeof(int));
            // If a texture is assigned, use it
            try {
                temp.texture = (*it)["texIndex"];
            } catch (const std::exception& e) {
                temp.texture = -1;
            }

            int tPointer = 0;
            int vPointer = 0;
            for (int i = 0; i < points.size(); i+= 3) {
                temp.vertices[vPointer] = (float) points[i];
                temp.vertices[vPointer + 1] = (float) points[i + 1];
                temp.vertices[vPointer + 2] = (float) points[i + 2];
                temp.vertices[vPointer + 3] = (float) colors[i];
                temp.vertices[vPointer + 4] = (float) colors[i + 1];
                temp.vertices[vPointer + 5] = (float) colors[i + 2];
                // If texture coordinates is supplied, use it
                if (!texCoord.empty()) {
                    temp.vertices[vPointer + 6] = (float) texCoord[tPointer];
                    temp.vertices[vPointer + 7] = (float) texCoord[tPointer + 1];
                } else {
                    temp.vertices[vPointer + 6] = -999;
                    temp.vertices[vPointer + 7] = -999;
                }

                if (!normals.empty()) {
                    temp.vertices[vPointer + 8] = (float) normals[i];
                    temp.vertices[vPointer + 9] = (float) normals[i+1];
                    temp.vertices[vPointer + 10] = (float) normals[i+2];
                }
                else {
                    temp.vertices[vPointer + 8] = 0;
                    temp.vertices[vPointer + 9] = 0;
                    temp.vertices[vPointer + 10] = -1.0;
                }

                tPointer+= 2;
                vPointer+= 11;
            }

            for (int i = 0; i < indices.size(); i++) {
                temp.indices[i] = indices[i];
            }

            this->add(temp);
        }
    }
}

void DrawHelper::loadTexture(const char * filename) {
    int width, height, nrChannels;

    // Load image
    unsigned char * data = stbi_load(filename, &width, &height, &nrChannels, 0);

    // Gen & bind texture
    textures.push_back(0);
    glGenTextures(1, &(textures[textures.size() - 1]));
    glBindTexture(GL_TEXTURE_2D, textures[textures.size() - 1]);

    // Parameters used specifying how the texture should be generated
    // Also tells how the mipmaps should be generated as well
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    if (data) {
        // Dunno why, but this 4 commands are useful
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
        glPixelStorei(GL_UNPACK_SKIP_PIXELS, 0);
        glPixelStorei(GL_UNPACK_SKIP_ROWS, 0);
        // The actual conversion from image to textures
        if (nrChannels == 4) {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        } else {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        }
        glGenerateMipmap(GL_TEXTURE_2D);
    } else {
        printf("[WARN] Failed to load %s ...\n",filename);
    }

    // Frees raw image data from memory, since it's texture is already built
    stbi_image_free(data);
}


void DrawHelper::draw(int i) {
    // If this polygon has a texture associated, use it
    if (poly[i].texture != -1) {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, textures[poly[i].texture]);
    }
    glBindVertexArray(vao[i]);
    glDrawElements(GL_TRIANGLES, poly[i].numOfIndex, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

void DrawHelper::drawAll() {
    for (int i = 0; i < poly.size(); i++) {
        this->draw(i);
    }
}

void DrawHelper::removeObject(int i) {
    ebo.erase(ebo.begin() + i);
    vbo.erase(vbo.begin() + i);
    vao.erase(vao.begin() + i);
    poly.erase(poly.begin() + i);
}

void DrawHelper::removeTexture(int i) {
    textures[i] = 0;
}
