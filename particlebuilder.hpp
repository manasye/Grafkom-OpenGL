#include <GL/gl.h>
#include <vector>

#include "particle.hpp"

#ifndef PARTICLE_BUILDER
#define PARTICLE_BUILDER

class ParticleBuilder 
{
    private:
        std::vector<Particle> particles;
        GLuint billboard_vertex_buffer;
        GLuint particles_position_buffer;
        GLuint particles_color_buffer;
        unsigned int count;
        unsigned int amount;
    public:
        ParticleBuilder(unsigned int);
};

#endif