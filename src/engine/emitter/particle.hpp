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

    Particle() : life(3.0f), rotation(glm::vec3(0.0f)), size(5.0f),
                 opacity(100.0f), color(glm::vec3(1.0f)) {
        
    };

    float life; // Particle life span in seconds

    // Rotation group, disabled for sphere types
    glm::vec3 rotation; // Rotation axis of particles
    // glm::vec3 rotationSpeed; // Rotation speed of particles

    float size; // Particle size
    float velocity;
    float opacity; // Particle opacity, range [0.0, 100.0]
    glm::vec3 color;
};
