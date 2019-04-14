#ifndef TEXTURE_H
#define TEXTURE_H

#include <GL/glew.h>

class Texture
{
    public:
        Texture(const char* fileName);
        ~Texture();
        void Bind();
    private:
        
}
