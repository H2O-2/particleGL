#pragma once

#include <glm/glm.hpp>

enum class ParticleType {
    // GLOW_SPHERE
    SPHERE,
    SQUARE,
    // TEXTURE
    TRIANGLE,
};

struct Particle {

    Particle(glm::vec3 velocity) : life(3.0f), color(glm::vec3(1.0f)), offset(glm::vec3(0.0f)), opacity(100.0f),
        rotation(glm::vec3(0.0f)), size(1.0f), velocity(velocity) {}

    float life; // Age of the particle in seconds
    glm::vec3 color; // Particle color
    glm::vec3 offset; // Particle positions
    float opacity; // Particle opacity, range [0.0, 100.0]

    // Rotation group, disabled for sphere types
    glm::vec3 rotation; // Rotation axis of particles
    // glm::vec3 rotationSpeed; // Rotation speed of particles

    float size; // Particle size
    glm::vec3 velocity; // Particle velocity
};
