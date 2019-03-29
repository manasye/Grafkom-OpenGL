#include <GL/glew.h>
#include <GL/glut.h>
#include <GL/gl.h>
#include <cstdio>
#include <vector>

#define WIDTH 800
#define HEIGHT 600

void display();
void init();

float vertices[] = {
    -0.5f, -0.5f, 0.0f, // left  
    0.5f, -0.5f, 0.0f, // right 
    0.0f,  0.5f, 0.0f  // top
};  

GLuint vbo, vao;
GLuint shaderProgram;

const char * vertexSource = R"glsl(
    #version 130

    in vec3 position;

    void main()
    {
        gl_Position = vec4(position, 1.0);
    }
    
)glsl";

const char * fragmentSource = R"glsl(
    #version 130

    out vec4 outColor;

    void main()
    {
        outColor = vec4(0.298f, 0.698f, 0.314f, 1.0f);
    }
)glsl";


int main(int argc, char* argv[])
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE|GLUT_RGB);
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

    GLint isCompiled = 0;

    // Compile vertex shader
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexSource, NULL);
    glCompileShader(vertexShader);
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &isCompiled);
    if (isCompiled == GL_FALSE)
    {   
        GLint maxLength = 0;
        glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &maxLength);

        // The maxLength includes the NULL character
        std::vector<GLchar> infoLog (maxLength);
        glGetShaderInfoLog(vertexShader, maxLength, &maxLength, &infoLog[0]);
        
        for (int i = 0; i < infoLog.size(); i++) {
            printf("%c",infoLog[i]);
        }
        printf("\n");

        // We don't need the shader anymore.
        glDeleteShader(vertexShader);

        // Use the infoLog as you see fit.
        // In this simple program, we'll just leave
        exit(0);
    }

    // Compile fragment shader
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
    glCompileShader(fragmentShader);
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &isCompiled);
    if (isCompiled == GL_FALSE)
    {   
        GLint maxLength = 0;
        glGetShaderiv(fragmentShader, GL_INFO_LOG_LENGTH, &maxLength);

        // The maxLength includes the NULL character
        std::vector<GLchar> infoLog (maxLength);
        glGetShaderInfoLog(fragmentShader, maxLength, &maxLength, &infoLog[0]);
        
        for (int i = 0; i < infoLog.size(); i++) {
            printf("%c",infoLog[i]);
        }
        printf("\n");

        // We don't need the shader anymore.
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);

        // Use the infoLog as you see fit.
        // In this simple program, we'll just leave
        exit(0);
    }

    // Link both previous shader to the "program"
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    //glBindFragDataLocation(shaderProgram, 0, "outColor");
    glLinkProgram(shaderProgram);

    glGetProgramiv(shaderProgram, GL_LINK_STATUS, (int *)&isCompiled);
    if (isCompiled == GL_FALSE)
    {
        GLint maxLength = 0;
        glGetProgramiv(shaderProgram, GL_INFO_LOG_LENGTH, &maxLength);

        // The maxLength includes the NULL character
        std::vector<GLchar> infoLog(maxLength);
        glGetProgramInfoLog(shaderProgram, maxLength, &maxLength, &infoLog[0]);
        
        // Don't leak shaders either.
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
        // We don't need the program anymore.
        glDeleteProgram(shaderProgram);

        // Print the log
        for (int i = 0; i < infoLog.size(); i++) {
            printf("%c",infoLog[i]);
        }
        printf("\n");
        
        // In this simple program, we'll just leave
        exit(0);
    }
    
    // Don't need these anymore
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);

    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    //GLint posAttrib = glGetAttribLocation(shaderProgram, "position");
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

}

void display()
{
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    glUseProgram(shaderProgram);
    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    glBindVertexArray(0);
    
    glBegin(GL_POLYGON);
    glColor3ub(76,175,80);
		glVertex2f(-0.9f, -0.8f);
		glVertex2f(-0.75f, -0.8f);
		glVertex2f(-0.6f, -0.7f);
		glVertex2f(-0.8f, -0.6f);
		glVertex2f(-0.95f, -0.7f);
    glEnd();

    glFlush();
}
