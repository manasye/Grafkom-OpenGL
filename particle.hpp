#include <glm/glm.hpp>

#ifndef PARTICLE_HPP
#define PARTICLE_HPP

struct Particle
{
    glm::vec3 pos, speed;
    glm::vec4 color;
    float cameradistance;
    float size, angle, weight;
    float life; //Remaining life of particle. If < 0 -> dead & unused
    
    Particle() : pos(0.0f), speed(0.0f), color(1.0f), size(0.0f), life(0.0f), cameradistance(-1.0f){}
    bool operator<(const Particle& other) const 
    {
        return this->cameradistance > other.cameradistance;
    }
};

#endif