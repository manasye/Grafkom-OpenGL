#include <GL/glew.h>
#include <GL/glut.h>
#include <GL/gl.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <cstdio>
#include <vector>
#include "filereader.cpp"
#include "shader.hpp"

#define FILENAME "data/pentagon.txt"
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
    glClearColor(0, 0, 0, 1.0f);
    glClearDepth(1.0f);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

    //GLfloat aspect = (GLfloat) WIDTH / (GLfloat) HEIGHT;
    
    glViewport(0,0,WIDTH,HEIGHT);
    glMatrixMode(GL_MODELVIEW); 
    glLoadIdentity();
    
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

    glm::mat4 view = glm::mat4(1.0f);
    glm::mat4 projection = glm::mat4(1.0f);
    glm::mat4 model = glm::mat4(1.0f);

    projection = glm::perspective(glm::radians(45.0f), (float) WIDTH / (float) HEIGHT, 0.1f, 100.0f);
    view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
    model = glm::rotate(model, glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f));

    glUniformMatrix4fv(glGetUniformLocation(shader->getProgram(),"model"),1,GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(glGetUniformLocation(shader->getProgram(),"view"),1,GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(glGetUniformLocation(shader->getProgram(),"projection"),1,GL_FALSE, glm::value_ptr(projection));
    
    glBindVertexArray(vao);
    glDrawElements(GL_TRIANGLES, poly.numOfIndex, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    glFlush();
}
