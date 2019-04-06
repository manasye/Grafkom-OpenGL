#ifndef SHADER_HPP
#define SHADER_HPP

#include <GL/glew.h>
#include <GL/gl.h>
#include <vector>
#include <iostream>
#include <fstream>

class Shader {
    public:
        // CALL THESE FUNCTIONS ONLY AFTER INITIALIZING GL
        Shader(const char * vertexSource, const char * fragmentSource);
        Shader(std::ifstream * vertexSourceFile, std::ifstream * fragmentSourceFile);
        ~Shader();
        void reload(const char * vertexSource, const char * fragmentSource);
        void reloadFromFileStream(std::ifstream * vertexSourceFile, std::ifstream * fragmentSourceFile);
        GLint isProgramCompiled();
        std::vector<GLchar> getCompileInfo();
        void printCompileInfo();
        GLuint getProgram();
        void useProgram();

    private:
        GLuint shaderProgram;
        GLint isCompiled;
        std::vector<GLchar> infoLog;

};

#include "shader.cpp"

#endif