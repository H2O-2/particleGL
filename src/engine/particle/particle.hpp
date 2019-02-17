#pragma once

#include <glm/glm.hpp>

class Particle {
public:
    Particle();
private:
    float life; // Particle life span in seconds
    float lifeRandom; // Randomness of particle life span in percentage(%), 0% means the particles genearted at the same time will all die at once and 100% means a possible range of [0.9*l0, 1.1*l0] where l0 is the life span specified
    float feather; // Level of feathering, only available for sphere type (currently)

    // Rotation group, disabled for sphere types
    glm::vec3 rotation; // Rotation axis of particles
    glm::vec3 rotationSpeed; // Rotation speed of particles
    float randomRotation; // Rotates the particle around a random axis, randomness in percentage(%) where 0% means 0 deg rotation for all particles and 100% means rotation in the range of [0, 360] deg
    // float randomRotationSpeed; // Rotates the particle around a random axis, randomness in percentage(%) where a 0% means no randomness in rotation speed and 100% means 

    float size; // Particle size
    float sizeRandom; // Randomness of particle size in percentage(%), 100% means a possible range of [0.2*s0, 5*s0] where s0 is the specified size
    /** TODO: size over life **/

    float opacity; // Particle opacity
    float opacityRandom; // Randomness of particle opacity in percentage(%), 100% means a possible range of [0, p0] where p0 is the specified opacity

    glm::vec3 color;
    /** TODO: color over life and random color **/
};
