#include <GL/glew.h>
#include <GL/glut.h>
#include <GL/gl.h>
#include <cstdio>
#include <vector>
#include "filereader.cpp"
#include "shader.hpp"

#define FILENAME "data/unicorn.txt"
#define WIDTH 800
#define HEIGHT 600

void display();
void init();

// VBO = vertex buffer object | VAO = vertex array object | EBO = element buffer object
GLuint vbo, vao, ebo;
Shader * shader;
Polygon poly;

int main(int argc, char *argv[])
{
    if (argc < 2) {
        poly = readPolygon(FILENAME);
    } else {
        poly = readPolygon(argv[1]);
    }

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(WIDTH, HEIGHT);
    glutCreateWindow("OpenGL Demo");

    glewInit();
    init();

    glutDisplayFunc(display);
    glutMainLoop();
    return 0;
}

void init()
{
    glClearColor(0, 0, 0, 0);
    glDisable(GL_DEPTH_TEST);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    //glOrtho(0.0, WIDTH-1, HEIGHT-1, 0, -1.0, 1.0);
    glMatrixMode(GL_MODELVIEW);

    std::ifstream vertexFile, fragmentFile;
    vertexFile.open("testvertex.vert");
    fragmentFile.open("testfragment.frag");

    shader = new Shader(&vertexFile, &fragmentFile);

    if (shader->isProgramCompiled() == GL_FALSE) {
        printf("Shader compilation failed...\n");
        shader->printCompileInfo();
        exit(0);
    }

    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ebo);

    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, poly.numOfVertex * (sizeof(float)), poly.vertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, poly.numOfIndex * (sizeof(int)), poly.indices, GL_STATIC_DRAW);
    //GLint posAttrib = glGetAttribLocation(shaderProgram, "position");
    // Positions
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);
    // Colors
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    // Binding position & color array to shader program
    glBindAttribLocation(shader->getProgram(), 0, "position");
    glBindAttribLocation(shader->getProgram(), 1, "inColor");

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void display()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    shader->useProgram();
    glBindVertexArray(vao);
    glDrawElements(GL_TRIANGLES, poly.numOfIndex, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    glFlush();
}
