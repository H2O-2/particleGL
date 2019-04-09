#pragma once

#include <glm/glm.hpp>

enum class ParticleType {
    // GLOW_SPHERE
    SPHERE,
    SQUARE,
    TRIANGLE,
    SPRITE
};

struct Particle {

    Particle(const float life, const glm::vec3& color, const glm::vec3& offset, const float opacity, const glm::vec3& rotation, const float size,
             const glm::vec3& velocity) :
        life(life), color(color), offset(offset), opacity(opacity), rotation(rotation), size(size),
        velocity(velocity) {}

    float life; // Age of the particle in seconds
    glm::vec3 color; // Particle color
    glm::vec3 offset; // Particle positions
    float opacity; // Particle opacity, range [0.0, 100.0]

    // Rotation group, disabled for sphere types
    glm::vec3 rotation; // Rotation axis of particles
    glm::vec3 rotationSpeed; // Rotation speed of particles

    float size; // Particle size
    glm::vec3 velocity; // Particle velocity

    // Since we assume that the camera cannot move, z coordinate is used instead of distance to camera
    // Particle further away from the camera will be smaller using this definition of <
    bool operator<(const Particle& p) const {
        return offset.z < p.offset.z;
    }
};
