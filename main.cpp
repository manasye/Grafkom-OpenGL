#include <GL/glew.h>
#include <GL/glut.h>
#include <GL/gl.h>
#include <cstdio>
#include <vector>

#define WIDTH 800
#define HEIGHT 600

void display();
void init();

// Pentagon vertices
float vertices[] = {
    // positions         // colors
    0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f,    // top
    -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,   // middle left
    -0.25f, -0.5f, 0.0f, 0.0f, 1.0f, 1.0f, // bottom left
    0.25f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f,  // bottom right
    0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f     // middle right
};

// Indexes, to tell how to draw the triangles to form a pentagon
unsigned int indices[] = {
    0, 1, 2,
    0, 2, 3,
    0, 3, 4};

// VBO = vertex buffer object | VAO = vertex array object | EBO = element buffer object
GLuint vbo, vao, ebo;
GLuint shaderProgram;

const char *vertexSource = R"glsl(
    #version 130

    in vec3 position;
    in vec3 inColor;

    out vec3 vertColor;

    void main()
    {
        gl_Position = vec4(position, 1.0);
        vertColor = inColor;
    }
    
)glsl";

const char *fragmentSource = R"glsl(
    #version 130

    in vec3 vertColor;
    out vec4 outColor;

    void main()
    {
        outColor = vec4(vertColor, 1.0f);
    }
)glsl";

int main(int argc, char *argv[])
{
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

    GLint isCompiled = 0;

    // Compile vertex shader
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexSource, NULL);
    glCompileShader(vertexShader);

    // Check shader status. If not compiled, terminate
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &isCompiled);
    if (isCompiled == GL_FALSE)
    {
        printf("Vertex shader failed to compile...\n");
        GLint maxLength = 0;
        glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &maxLength);

        // The maxLength includes the NULL character
        std::vector<GLchar> infoLog(maxLength);
        glGetShaderInfoLog(vertexShader, maxLength, &maxLength, &infoLog[0]);

        for (int i = 0; i < infoLog.size(); i++)
        {
            printf("%c", infoLog[i]);
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

    // Check shader status
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &isCompiled);
    if (isCompiled == GL_FALSE)
    {
        printf("Fragment shader failed to compile...\n");
        GLint maxLength = 0;
        glGetShaderiv(fragmentShader, GL_INFO_LOG_LENGTH, &maxLength);

        // The maxLength includes the NULL character
        std::vector<GLchar> infoLog(maxLength);
        glGetShaderInfoLog(fragmentShader, maxLength, &maxLength, &infoLog[0]);

        for (int i = 0; i < infoLog.size(); i++)
        {
            printf("%c", infoLog[i]);
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

    // Check shader status
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, (int *)&isCompiled);
    if (isCompiled == GL_FALSE)
    {
        printf("Shader program failed to link...\n");
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
        for (int i = 0; i < infoLog.size(); i++)
        {
            printf("%c", infoLog[i]);
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
    glGenBuffers(1, &ebo);

    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    //GLint posAttrib = glGetAttribLocation(shaderProgram, "position");
    // Positions
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);
    // Colors
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    // Binding position & color array to shader program
    glBindAttribLocation(shaderProgram, 0, "position");
    glBindAttribLocation(shaderProgram, 1, "inColor");

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void display()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    glUseProgram(shaderProgram);
    glBindVertexArray(vao);
    glDrawElements(GL_TRIANGLES, 9, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    /*
    glBegin(GL_POLYGON);
    glColor3ub(76,175,80);
		glVertex2f(-0.9f, -0.8f);
		glVertex2f(-0.75f, -0.8f);
		glVertex2f(-0.6f, -0.7f);
		glVertex2f(-0.8f, -0.6f);
		glVertex2f(-0.95f, -0.7f);
    glEnd();
    */

    glFlush();
    //glFinish();
}
