#pragma once

#include "particle.hpp"
#include "../geometries/sphere.hpp"
#include "../geometries/square.hpp"
#include "../geometries/triangle.hpp"
#include "../../util/randGen.hpp"

// Shape of particle emitter
enum class EmitterType {
    POINT, // Particles will emit from one point, this is the default setting
    BOX, // Particles will be born in random position inside a box
    SPHERE, // Particles will be born in random position inside a sphere
};

// Type of emission done by the emitter
enum class EmitterDirection {
    UNIFORM, // Random direction, this is the default setting
    DIRECTIONAL, // A certain direction, defaults to the direction of y axis
};

// How emitter size can be specified. This is not applicable to point type emitter
// When switching from INDIVIDUAL to LINKED, the size value on X direction will be used
enum class EmitterSize {
    LINKED, // Size is the same on all directions (X, Y, Z)
    INDIVIDUAL // Size can be modified to different values on different directions
};

// Rendering mode for particles.
// Uniform Model will pass to shader only instanced translation, rotation and scale will be passed as uniform variables while Varing model will pass instanced model matrices to shader
// Uniform Color will only pass color to fragment shdaer as uniform variable while Varing color will pass instanced colors to (vertex) shader
enum class RenderMode {
    U_MODEL_U_COLOR, // Uniform Model Uniform Color
    U_MODEL_V_COLOR, // Uniform Model Varing Color
    V_MODEL_U_COLOR, // Varing Model Uniform Color
    V_MODEL_V_COLOR  // Uniform Model Varing Color
};

extern const RenderMode DEFAULT_RENDER;
extern const int INIT_PARTICLE_PER_SEC;
extern const ParticleType INIT_PARTICLE_TYPE;
extern const float INIT_DIR_SPREAD;
extern const EmitterDirection INIT_EMIT_DIR;
extern const glm::vec3 INIT_EMITTER_POSN;
extern const glm::vec3 INIT_EMITTER_ROTATION;
extern const glm::vec3 INIT_EMITTER_SIZE;
extern const float EMITTER_SIZE_SCALE;
extern const EmitterSize INIT_EMITTER_SIZE_TYPE;
extern const EmitterType INIT_EMITTER_TYPE;
extern const int MAX_PARTICLE_NUM;
extern const float PERCENTAGE_SCALE;

extern const glm::vec3 INIT_PARTICLE_COLOR;
extern const float INIT_COLOR_RANDOMNESS;
extern const float INIT_PARTICLE_FEATHER;
extern const float INIT_FEATHER_RANDOMNESS;
extern const float INIT_PARTICLE_LIFE;
extern const float INIT_LIFE_RANDOMNESS;
extern const float INIT_PARTICLE_OPACITY;
extern const float INIT_OPACITY_RANDOMNESS;
extern const glm::vec3 INIT_PARTICLE_ROTATION;
extern const float INIT_ROTATION_RANDOMNESS;
extern const float INIT_PARTICLE_SIZE;
extern const float INIT_SIZE_RANDOMNESS;
extern const float INIT_VELOCITY;
extern const float INIT_VELOCITY_RANDOMNESS;
extern const float INIT_VELOCITY_RANDOMNESS_DIST;
extern const float PARTICLE_SIZE_SCALE;
extern const float PARTICLE_VELOCITY_SCALE;

class Emitter {
public:
    typedef std::vector<Particle> Particles;

    Emitter(const float& secondPerFrame);
    ~Emitter();

    /***** Emitter Attributes *****/
    EmitterDirection getEmitterDirectionType();
    EmitterDirection* getEmitterDirectionTypePtr();
    float* getEmitterDirectionSpreadPtr();

    const glm::vec3& getEmitterPosn() const;
    float* getEmitterPosnPtr();

    const glm::vec3& getEmitterRotation() const;
    float* getEmitterRotationPtr();

    const glm::vec3& getEmitterSize() const;
    float* getEmitterSizePtr();
    EmitterSize getEmitterSizeType() const;
    EmitterSize* getEmitterSizeTypePtr();

    EmitterType getEmitterType() const;
    EmitterType* getEmitterTypePtr();



    /***** Particle Attributes *****/
    size_t getParticleNum() const;

    uint32_t* getParticlesPerSecPtr();

    ParticleType getParticleType() const;
    void setParticleType(ParticleType particleType);

    const glm::vec4 getParticleColorAndOpacity() const;
    float* getParticleColorPtr();
    float* getParticleColorRandomnessPtr();

    float* getParticleLifePtr();
    float* getParticleLifeRandomnessPtr();

    float* getParicleOpacityPtr();
    float* getParicleOpacityRandomnessPtr();

    const glm::vec3& getParticleRotation() const;
    float getParticleRotationRandomness() const;
    void setParticleRotationRandomness(const float particleRotationRandom);

    float getParticleSize() const;
    float* getParticleSizePtr();
    float* getParticleSizeRandomnessPtr();

    float* getInitialVelocityPtr();
    float* getInitialVelocityRandomnessPtr();
    float* getInitialVelocityRandomnessDistributionPtr();



    /***** User Unmodifiables *****/
    float getBaseScale() const;
    GLenum getDrawMode() const;
    int getIndexNum() const;
    int getCurrentParticleNum() const;
    const std::vector<float>& getInstancedColors() const;
    const std::vector<float>& getOffsets() const;
    const std::vector<float>& getModelMatrices() const;
    RenderMode getRenderMode() const;
    uint32_t getVAO() const;

    bool useEBO() const;
    void update(const float& deltaTime);
private:
    /**
     * Note: All randomness and percentage values is a hundredth of their displayed value
    **/

    /***** Emitter Attributes *****/
    EmitterDirection direction; // Emitter direction
    float directionSpread; // Spread of the particle beam in percentage(%), this is disabled for uniform emitter direction
    EmitterType emitterType; // Emitter type (i.e. shape of the emitter)

    float initVelocity; // Initial velocity of particles measured in seconds
    float initVelocityPerFrame; // Initial velocity of particles measured in frames
    float initVelocityRandom; // Randomness of velocity in percentage(%), 0% means no change of velocity to all particles and r% means a possible range of ((1-r%)*v0, (1+r%)*v0] of velocity where v0 is the velocity specified
    float initVelocityRandomDistribution; // Portion of the random generated initial velocity that is higher than the given initial velocity.
    // NOTE: The initVelocityRandomDistribution behaves differently from the Velocity Distribution in Particular as I found it couterintuitive

    uint32_t particlesPerSec; // Number of particles generated per second

    glm::vec3 position; // Position of emitter
    glm::vec3 rotation; // Rotation of emitter
    glm::vec3 size; // Size of emitter, this is disabled for point emitters
    EmitterSize emitterSizeType; // Specifies how emitter size can be modified, this is disabled for point emitters

    RandGen randGen; // Random number generator



    /****** Particle Attributes ******/
    float feather; // Level of feathering, only available for sphere type (currently)

    glm::vec3 particleColor; // Color of particles
    float particleColorRandom; // Randomness of color in percentage(%), 0% means no change of color to all particles and 100% means any color is possible

    float particleLife; // Particle life span in seconds
    float particleLifeRandom; // Randomness of particle life span in percentage(%), 0% means the particles genearted at the same time will all die at once and 100% means a possible range of [0.9*l0, 1.1*l0] where l0 is the life span specified

    float particleOpacity; // Opacity of particles in percentage
    float particleOpacityRandom; // Randomness of particle opacity in percentage(%), 100% means a possible range of [0, p0] where p0 is the specified opacity

    glm::vec3 particleRotation;
    float particleRotationRandom; // Rotates the particle around a random axis, randomness in percentage(%) where 0% means 0 deg rotation for all particles and 100% means rotation in the range of [0, 360] deg
    // float randomRotationSpeed; // Rotates the particle around a random axis, randomness in percentage(%) where a 0% means no randomness in rotation speed and 100% means

    float particleSize;
    float particleSizeRandom; // Randomness of particle size in percentage(%), 100% means a possible range of [0.2*s0, 5*s0] where s0 is the specified size

    float particlesPerFrame; // Number of particles generated per frame, i.e. in `secondPerFrame` second, this could be smaller than one
    ParticleType particleType; // Shape of particles

    /** TODO: size over life **/
    /** TODO: color over life and random color **/
    /** TODO: Pre Run **/



    /***** User Unmodifiables *****/
    std::unique_ptr<Geometry> curGeomtry; // Current geometry shape of the particles
    Particles particles; // All particles generated by this emitter
    int activeParticleNum; // Number of particles to be rendered
    float secondPerFrame; // Time of one frame in second
    RenderMode emitterRenderMode;
    glm::mat4 emitterRotation; // Rotation matrix for emitter
    glm::mat4 emitterTranslation; // Translation matrix for emitter

    uint16_t lastUsedParticle; // Last used particle
    std::vector<float> colors; // Offsets of particles
    std::vector<float> modelMatrices; // Offsets of particles
    std::vector<float> offsets; // Offsets of particles
    float newParticleNumBuffer; // Buffers the current `number` of particles to be emitted. This value is always smaller than one after each update

    glm::vec3 generateInitialParticlePosn(); // Return a birth position for some particle according to current emitter type
    glm::vec3 generateInitialParticleVelocity(); // Return a velocity for some particle according to current emitter direction
    int getFirstUnusedParticle(); // Get the index of first unused particle in particles vector
    void setGeometry(ParticleType particleType); // Buffer the geometry and GPU data for geometry of given type
    void updateRenderMode();
};
