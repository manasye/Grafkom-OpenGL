#include <GL/glut.h>
#include <GL/gl.h>

#define WIDTH 800
#define HEIGHT 600

void display();
void init();

int main(int argc, char* argv[])
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE|GLUT_RGB);
    glutInitWindowSize(WIDTH, HEIGHT);
    glutCreateWindow("OpenGL Demo");

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
    glOrtho(0.0, WIDTH-1, HEIGHT-1, 0, -1.0, 1.0);
    glMatrixMode(GL_MODELVIEW);
}

void display()
{
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    glBegin(GL_POLYGON);
    glColor3ub(76,175,80);
		glVertex2f(100.0f, 460.0f);
		glVertex2f(80.0f, 410.0f);
		glVertex2f(150.0f, 360.0f);
		glVertex2f(220.0f, 410.0f);
		glVertex2f(200.0f, 460.0f);
    glEnd();

    glFlush();
}
