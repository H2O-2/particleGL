#pragma once

#include "particle.hpp"
#include "../geometries/sphere.hpp"
#include "../geometries/square.hpp"
#include "../geometries/triangle.hpp"
#include "../../util/makeUnique.hpp"
#include "../../util/randGen.hpp"

// Shape of particle emitter
enum class EmitterType {
    POINT,
    BOX,
    SPHERE,
};

// Type of emission done by the emitter
enum class EmitterDirection {
    UNIFORM, // Random direction, this is the default setting
    DIRECTIONAL, // A certain direction, defaults to the direction of y axis
};

// Type of blend between particles
enum class ParticleBlend {
    NONE,
    ADD,
    SCREEN,
    LIGHTEN
};

extern const int INIT_PARTICLE_PER_SEC;
extern const ParticleBlend INIT_BLEND_TYPE;
extern const ParticleType INIT_PARTICLE_TYPE;
extern const EmitterDirection INIT_EMIT_DIR;
extern const float INIT_DIR_SPREAD;
extern const EmitterType INIT_EMITTER_TYPE;
extern const glm::vec3 INIT_EMITTER_POSN;
extern const glm::vec3 INIT_PARTICLE_ROTATION;
extern const glm::vec3 INIT_PARTICLE_SIZE;
extern const float INIT_FEATHER;
extern const float INIT_VELOCITY;
extern const float INIT_LIFE;

class Emitter {
public:
    typedef std::vector<Particle> Particles;

    ParticleType newParticleType;

    Emitter(const float& secondPerFrame);
    ~Emitter();

    uint32_t getParticlesPerSec() const;
    void setParticlesPerSec(const uint32_t& particlesPerSec);
    ParticleType getParticleType() const;
    void setParticleType(ParticleType particleType);
    ParticleBlend getBlendType() const;
    void setBlendType(ParticleBlend blendType);
    void setType(EmitterType emitterType);
    void setEmitDirection(EmitterDirection direction);
    void setDirectionSpread(const float& directionSpread);
    glm::vec3 getPosn() const;
    void setPosn(const glm::vec3& posn);
    glm::vec3 getRotation() const;
    void setRotation(const glm::vec3& rotation);
    glm::vec3 getSize() const;
    void setSize(const glm::vec3& size);
    void setInitialVelocity(const float& initVelocity);
    void setInitialVelocityRandomness(const float& initVelocityRandom);
    void setLifeRandomness(const int& lifeRandom);
    void setSizeRandomness(const int& sizeRandom);
    void setRotationRandomness(const int& rotationRandom);
    void setOpacityRandomness(const int& opacityRandom);

    float getBaseScale() const;
    GLenum getDrawMode() const;
    int getIndexNum() const;
    std::vector<float> getOffsets();
    glm::mat4* getModelMatrices() const;
    uint32_t getVAO() const;

    bool useEBO() const;
    void update(const float& deltaTime);
private:
    /***** DEBUG *****/
    uint32_t particleAmount;
    /***** DEBUG *****/

    Particles particles; // All particles generated by this emitter

    ParticleBlend blendType; // Type of blend applied to particles
    uint32_t particlesPerSec; // Number of particles generated per second
    ParticleType particleType; // Shape of particles

    EmitterDirection direction; // Emitter direction
    float directionSpread; // Spread of the particle beam in percentage(%), this is disabled for uniform emitter direction
    EmitterType emitterType; // Emitter type

    std::unique_ptr<Geometry> curGeomtry; // Current geometry shape of the particles

    glm::vec3 position; // Position of emitter
    glm::vec3 rotation; // Rotation of emitter
    glm::vec3 size; // Size of emitter, this is disabled for point emitters

    float feather; // Level of feathering, only available for sphere type (currently)

    float initVelocity; // Initial velocity of particles
    int initVelocityRandom; // Randomness of velocity in percentage(%), 0% means no change of velocity to all particles and 100% means a possible range of [0, 2*v0] of velocity where v0 is the velocity specified

    float particleLife; // Particle life span in seconds
    int lifeRandom; // Randomness of particle life span in percentage(%), 0% means the particles genearted at the same time will all die at once and 100% means a possible range of [0.9*l0, 1.1*l0] where l0 is the life span specified

    int opacityRandom; // Randomness of particle opacity in percentage(%), 100% means a possible range of [0, p0] where p0 is the specified opacity
    int rotationRandom; // Rotates the particle around a random axis, randomness in percentage(%) where 0% means 0 deg rotation for all particles and 100% means rotation in the range of [0, 360] deg
    int sizeRandom; // Randomness of particle size in percentage(%), 100% means a possible range of [0.2*s0, 5*s0] where s0 is the specified size
    // float randomRotationSpeed; // Rotates the particle around a random axis, randomness in percentage(%) where a 0% means no randomness in rotation speed and 100% means

    float secondPerFrame; // Time of one frame in second

    RandGen randGen; // Random number generator

    /** TODO: size over life **/
    /** TODO: color over life and random color **/

    /** TODO: Pre Run **/


    void setGeometry(ParticleType particleType); // Buffer the geometry and GPU data for geometry of given type
};
