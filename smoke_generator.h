#ifndef SMOKE_GENERATOR_H
#define SMOKE_GENERATOR_H
#include "particle_generator.hpp"

class SmokeGenerator : public ParticleGenerator {
public:
  // Constructor
  SmokeGenerator(const char texture_path[], unsigned int amount);

  // Update all particles
  void update(float dt, unsigned int new_particles, glm::vec3 camera_position, glm::vec3 offset = glm::vec3(0.0f, 0.0f, 0.0f));

protected:
  // Respawns particle
  void respawnParticle(Particle &particle, glm::vec3 offset = glm::vec3(0.0f, 0.0f, 0.0f));

};

#include "smoke_generator.cpp"

#endif
