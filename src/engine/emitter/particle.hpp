#pragma once

#include <glm/glm.hpp>

enum class ParticleType {
    // GLOW_SPHERE
    SPHERE,
    SQUARE,
    TRIANGLE,
    SPRITE
};

/*****
 * NOTE: Instead of storing actual values of particle attributes, a base value of each attribute is stored in
 * the emitter and here the offsets of those values are stored. E.g. for a particle attribute with uniform
 * transformation (i.e. without randomness) the corresponding offset stored in this struct would be 0.
 *
 * Exceptions for this are particle life and particle color. For the former only the actual value is stored
 * (thus the life of a created particle will not change). For the latter a random color generated (or the original
 * color if there is no randomness in color) is stored and later an interpolation would be done between the random
 * color and the original color
 *****/

struct Particle {

    Particle(const float life, const glm::vec3& lerpColor, const glm::vec3& offset,
             const float opacityOffset, const glm::vec3& rotationOffset, const float sizeOffset, const glm::vec3& velocity) :
        life(life), lerpColor(lerpColor), offset(offset), opacityOffset(opacityOffset), rotationOffset(rotationOffset),
        sizeOffset(sizeOffset), velocity(velocity) {}

    float life; // Age of the particle in seconds
    glm::vec3 lerpColor; // Particle color
    glm::vec3 offset; // Particle positions
    float opacityOffset; // Particle opacity, range [0.0, 100.0]

    // Rotation group, disabled for sphere types
    glm::vec3 rotationOffset; // Rotation axis of particles
    glm::vec3 rotationSpeed; // Rotation speed of particles

    float sizeOffset; // Particle size offset
    glm::vec3 velocity; // Particle velocity

    // Since we assume that the camera cannot move, z coordinate is used instead of distance to camera
    // Particle further away from the camera will be smaller using this definition of <
    bool operator<(const Particle& p) const {
        return offset.z < p.offset.z;
    }
};
