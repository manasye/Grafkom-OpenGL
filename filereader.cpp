#ifndef __FILEREADER_CPP
#define __FILEREADER_CPP

#include <stdlib.h>
#include <stdio.h>
#include "polygon.hpp"
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include "lib/json.hpp"

using namespace std;
using namespace nlohmann;

std::vector<Polygon> * readPolygon(char * fileName)
{
    char temp;
    int numOfVertex = 6;
    int numOfIndex = 3;
    ifstream inputFile;
    json inputJson;

    std::vector<Polygon> * poly = new std::vector<Polygon>();

    inputFile.open(fileName);
    inputFile >> inputJson;

    //cout << inputJson.dump();

    json textures = inputJson["textures"];
    json objects = inputJson["objects"];

    if (!textures.empty()) {
        // will be implemented later
    }

    if (!objects.empty()) {
        for (json::iterator it = objects.begin(); it != objects.end(); ++it) {
            Polygon temp;
            json points = (*it)["points"];
            json colors = (*it)["colors"];
            json indices = (*it)["indices"];

            temp.vertices = (float *) malloc(points.size() * 2 * sizeof(float));
            temp.numOfVertex = 2 * points.size();
            temp.indices = (unsigned int *) malloc(indices.size() * sizeof(int));
            temp.numOfIndex = indices.size();
            
            for (int i = 0; i < points.size(); i+= 3) {
                temp.vertices[(i * 2)] = (float) points[i];
                temp.vertices[(i * 2) + 1] = (float) points[i + 1];
                temp.vertices[(i * 2) + 2] = (float) points[i + 2];
                temp.vertices[(i * 2) + 3] = (float) colors[i];
                temp.vertices[(i * 2) + 4] = (float) colors[i + 1];
                temp.vertices[(i * 2) + 5] = (float) colors[i + 2];
            }
            for (int i = 0; i < indices.size(); i++) {
                temp.indices[i] = indices[i];
            }

            /*
            for (int i = 0; i < temp.numOfVertex; i++) {
                cout << temp.vertices[i] << " ";
            }
            cout << endl;
            */
            poly->push_back(temp);
        }
    }
    
    return poly;
}

#endif