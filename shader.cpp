#include "shader.hpp"
#include <string>
#include <sstream>

Shader::Shader(const char * vertexSource, const char * fragmentSource) {
    this->reload(vertexSource, fragmentSource);
}

Shader::Shader(std::ifstream * vertexSourceFile, std::ifstream * fragmentSourceFile) {
    this->reloadFromFileStream(vertexSourceFile, fragmentSourceFile);
}

Shader::~Shader() {
    glDeleteProgram(shaderProgram);
}

void Shader::reload(const char * vertexSource, const char * fragmentSource) {
    // Compile vertex shader
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexSource, NULL);
    glCompileShader(vertexShader);

    // Check shader status. If not compiled, terminate
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &(this->isCompiled));
    if (this->isCompiled == GL_FALSE)
    {
        printf("Vertex shader failed to compile...\n");
        GLint maxLength = 0;
        glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &maxLength);

        // The maxLength includes the NULL character
        this->infoLog.resize(maxLength);
        glGetShaderInfoLog(vertexShader, maxLength, &maxLength, &(this->infoLog[0]));

        // Don't need the shader anymore.
        glDeleteShader(vertexShader);

        return;
    }

    // Compile fragment shader
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
    glCompileShader(fragmentShader);

    // Check shader status
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &(this->isCompiled));
    if (this->isCompiled == GL_FALSE)
    {
        printf("Fragment shader failed to compile...\n");
        GLint maxLength = 0;
        glGetShaderiv(fragmentShader, GL_INFO_LOG_LENGTH, &maxLength);

        // The maxLength includes the NULL character
        this->infoLog.resize(maxLength);
        glGetShaderInfoLog(fragmentShader, maxLength, &maxLength, &(this->infoLog[0]));

        // We don't need the shader anymore.
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);

        return;
    }

    // Link both previous shader to the "program"
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    //glBindFragDataLocation(shaderProgram, 0, "outColor");
    glLinkProgram(shaderProgram);

    // Check shader status
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, (int *) &(this->isCompiled));
    if (isCompiled == GL_FALSE)
    {
        printf("Shader program failed to link...\n");
        GLint maxLength = 0;
        glGetProgramiv(shaderProgram, GL_INFO_LOG_LENGTH, &maxLength);

        // The maxLength includes the NULL character
        this->infoLog.resize(maxLength);
        glGetProgramInfoLog(shaderProgram, maxLength, &maxLength, &(this->infoLog[0]));

        // Don't leak shaders either.
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
        // We don't need the program anymore.
        glDeleteProgram(shaderProgram);

        return;
    }

    // Don't need these anymore
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

void Shader::reloadFromFileStream(std::ifstream * vertexSourceFile, std::ifstream * fragmentSourceFile) {
    // Dunno why I have to make it like this.
    // "Cutting" shortcuts (like making "vertexStream.str()" one line with ".c_str()") makes it parse incorrect string 
    std::stringstream vertexStream, fragmentStream;
    vertexStream << vertexSourceFile->rdbuf();
    fragmentStream << fragmentSourceFile->rdbuf();

    std::string vertexString = vertexStream.str();
    std::string fragmentString = fragmentStream.str();
    
    const char * vertexSource = vertexString.c_str();
    const char * fragmentSource = fragmentString.c_str();

    this->reload(vertexSource, fragmentSource);
}

GLint Shader::isProgramCompiled() {
    return isCompiled;
}

std::vector<GLchar> Shader::getCompileInfo() {
    return infoLog;
}

void Shader::printCompileInfo() {
    for (int i = 0; i < infoLog.size(); i++) {
        printf("%c",infoLog[i]);
    }
    printf("\n");
}


GLuint Shader::getProgram() {
    return shaderProgram;
}

void Shader::useProgram() {
    glUseProgram(shaderProgram);
}