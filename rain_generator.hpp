#ifndef RAIN_GENERATOR_H
#define RAIN_GENERATOR_H
#include "particle_generator.hpp"

class RainGenerator : public ParticleGenerator {
public:
  // Constructor
  RainGenerator(const char texture_path[], unsigned int amount);

  // Update all particles
  void update(float dt, unsigned int new_particles, glm::vec3 camera_position, glm::vec3 offset = glm::vec3(0.0f, 0.0f, 0.0f));

protected:
  // Respawns particle
  void respawnParticle(Particle &particle, glm::vec3 offset = glm::vec3(0.0f, 0.0f, 0.0f));

};

#include "rain_generator.cpp"

#endif
