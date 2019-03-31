#include <stdlib.h>
#include <stdio.h>
#include "polygon.hpp"
#include <iostream>

using namespace std;

float* readVertices(FILE* inputFile, float* vertices, int* numOfVertex);
unsigned int* readIndices(FILE* inputFile, unsigned int * indices, int* numOfIndex, char* temp);
float generateRand(); //Random inclusive from 0 to 1
int seekToEOL(FILE *inputFile);

Polygon readPolygon(char* fileName)
{
    char temp;
    int numOfVertex = 6;
    int numOfIndex = 3;

    FILE* inputFile = fopen(fileName, "r");

    float *vertices = (float*) malloc(sizeof(float) * (numOfVertex));
    unsigned int *indices = (unsigned int*) malloc(sizeof(unsigned int) * (numOfIndex));

    temp = getc(inputFile);

    while (temp != EOF)
    {
        if (temp == '#')
        {
            temp = seekToEOL(inputFile);
        }
        else if ((temp == 'V') || (temp == 'v'))
        {
            vertices = readVertices(inputFile, vertices, &numOfVertex);
            temp = getc(inputFile);
        }
        else if ((temp == 'P') || (temp == 'p'))
        {
            indices = readIndices(inputFile, indices, &numOfIndex, &temp);
        }
        else
        {
            temp = getc(inputFile);
        }
    }

    numOfVertex-=6;
    numOfIndex-=3;

    vertices = (float*) realloc(vertices, sizeof(float) * (numOfVertex));
    indices = (unsigned int*) realloc(indices, sizeof(unsigned int) * (numOfIndex));

    fclose(inputFile);

    Polygon poly = {
        vertices,
        indices,
        numOfVertex,
        numOfIndex
    };

    return poly;
}

float* readVertices(FILE* inputFile, float* vertices, int* numOfVertex)
{
    int res;
    float x, y;

    while ((res = fscanf(inputFile, "(%f,%f)\n", &x, &y)) != EOF)
    {
        if (res == 2)
        {
            //Position
            vertices[*numOfVertex-6] = x;
            vertices[*numOfVertex-5] = y;
            vertices[*numOfVertex-4] = 0.0f;

            //Color
            vertices[*numOfVertex-3] = generateRand();
            vertices[*numOfVertex-2] = generateRand();
            vertices[*numOfVertex-1] = generateRand();

            *numOfVertex+=6;
            vertices = (float*) realloc(vertices, (*numOfVertex) * sizeof(float));

            fgetc(inputFile);
        } else
        {
            break;
        }
    }

    return vertices;
}

unsigned int* readIndices(FILE* inputFile, unsigned int * indices, int* numOfIndex, char* temp)
{
    unsigned int v1, v2, v3;
    int res;
    fgetc(inputFile);
    while ((res = fscanf(inputFile, "(%d,%d,%d)\n", &v1, &v2, &v3)) != EOF)
    {
        if (res == 3)
        {
            indices[*numOfIndex-3] = v1;
            indices[*numOfIndex-2] = v2;
            indices[*numOfIndex-1] = v3;

            *numOfIndex+=3;
            indices = (unsigned int*) realloc(indices, *numOfIndex * sizeof(unsigned int));

            fgetc(inputFile);
        } else
        {
            break;
        }
    }

    if (res == EOF)
    {
        *temp = res;
    }

    return indices;
}

int seekToEOL(FILE* inputFile)
{
    char c = getc(inputFile);
    while ((c != '\n') && (c != EOF))
    {
        c = getc(inputFile);
    }
    return c;
}

float generateRand()
{
    return static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
}
