#include <GL/glew.h>
#include <GL/glut.h>
#include <GL/gl.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <cstdio>
#include <vector>
#include "drawhelper.cpp"
#include "shader.hpp"

#define FILENAME "data/car.txt"
#define WIDTH 800
#define HEIGHT 600

void handleMouseClick(int button, int state, int x, int y);
void handleMouseMove(int x, int y);
void handleSpecialKeyboard(int key, int x, int y);
void display();
void init();

// VBO = vertex buffer object | VAO = vertex array object | EBO = element buffer object
Shader *shader;
DrawHelper drawer;

// Camera "distance" to object
float radius = 5.0f;
float mouseSensitivity = 0.05f;
bool isDragging = false;
int rotDX, rotDY, prevX, prevY;
float prevRotX = 0.0f;
float prevRotZ = 0.0f;

// Keyboard rotation attribute
int deltaKeyboard = 1;
float keyboardSensitivity = 0.1f;

// Camera position
glm::vec3 cameraPos = glm::vec3(sin(prevRotX) * radius, 0.0f, cos(prevRotZ) * radius);

int g_argc;
char **g_argv;

int main(int argc, char *argv[])
{
    g_argc = argc;
    g_argv = argv;
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(WIDTH, HEIGHT);
    glutCreateWindow("OpenGL Demo");

    glewInit();
    init();

    glutDisplayFunc(display);
    glutMouseFunc(handleMouseClick);
    glutSpecialFunc(handleSpecialKeyboard);
    glutMotionFunc(handleMouseMove);
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

    //glViewport(0,0,WIDTH,HEIGHT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    std::ifstream vertexFile, fragmentFile;
    vertexFile.open("testvertex.vert");
    fragmentFile.open("testfragment.frag");

    shader = new Shader(&vertexFile, &fragmentFile);

    if (shader->isProgramCompiled() == GL_FALSE)
    {
        printf("Shader compilation failed...\n");
        shader->printCompileInfo();
        exit(0);
    }

    if (g_argc < 2)
    {
        drawer.addFromFile(FILENAME);
        drawer.addFromFile("data/car.json");
    }
    else
    {
        for (int i = 1; i < g_argc; i++)
        {
            drawer.addFromFile(g_argv[i]);
        }
    }
    //drawer.remove(0);
}

void handleMouseMove(int x, int y)
{
    if (isDragging)
    {
        rotDX = prevX - x;
        rotDY = prevY - y;
        prevX = x;
        prevY = y;
        prevRotX += ((float)rotDX * mouseSensitivity);
        prevRotZ += ((float)rotDX * mouseSensitivity);
        cameraPos = glm::vec3(sin(prevRotX) * radius, 0.0f, cos(prevRotZ) * radius);
        glutPostRedisplay();
    }
}

void handleSpecialKeyboard(int key, int x, int y)
{
    rotDX = deltaKeyboard;
    rotDY = deltaKeyboard;
    if (key == GLUT_KEY_LEFT)
    {
        prevRotX += ((float)rotDX * keyboardSensitivity);
        prevRotZ += ((float)rotDX * keyboardSensitivity);
    }
    else if (key == GLUT_KEY_RIGHT)
    {
        prevRotX -= ((float)rotDX * keyboardSensitivity);
        prevRotZ -= ((float)rotDX * keyboardSensitivity);
    }
    cameraPos = glm::vec3(sin(prevRotX) * radius, 0.0f, cos(prevRotZ) * radius);
    glutPostRedisplay();
}

void handleMouseClick(int button, int state, int x, int y)
{
    if (button == GLUT_RIGHT_BUTTON)
    {
        if (state == GLUT_DOWN)
        {
            isDragging = true;
            prevX = x;
            prevY = y;
        }
        else // GLUT_UP
        {
            isDragging = false;
        }
    }
}

void display()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    shader->useProgram();

    glm::vec3 cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

    glm::mat4 view = glm::lookAt(cameraPos, cameraTarget, cameraUp);
    glm::mat4 projection = glm::mat4(1.0f);
    glm::mat4 model = glm::mat4(1.0f);

    projection = glm::perspective(glm::radians(40.0f), (float)WIDTH / (float)HEIGHT, 0.1f, 100.0f);
    model = glm::rotate(model, glm::radians(-85.0f), glm::vec3(1.0f, 0.0f, 0.0f));

    glUniformMatrix4fv(glGetUniformLocation(shader->getProgram(), "model"), 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(glGetUniformLocation(shader->getProgram(), "view"), 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(glGetUniformLocation(shader->getProgram(), "projection"), 1, GL_FALSE, glm::value_ptr(projection));

    drawer.drawAll();

    glFlush();
}
