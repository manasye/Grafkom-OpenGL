#include <GL/gl.h>
#include <vector>

#include "particle.hpp"

#ifndef PARTICLE_BUILDER
#define PARTICLE_BUILDER

GLuint loadTextureFromFile(const char*);

class ParticleBuilder 
{
    private:
        std::vector<Particle> particles;
        std::vector<float> g_particle_position_size_data;
        std::vector<unsigned char> g_particle_color_data;
        GLuint billboard_vertex_buffer;
        GLuint particles_position_buffer;
        GLuint particles_color_buffer;
        GLuint vao;
        GLuint texture; //save texture (rain/smoke)
        unsigned int count;
        unsigned int amount;
        unsigned int last_used_particle;
    public:
        ParticleBuilder();
        ParticleBuilder(unsigned int, const char*);
        void draw();
        unsigned int findUnusedParticle();
        void sortParticles();
        void update(float, glm::vec3);
};

#endif