#include "emitter.hpp"

#define _USE_MATH_DEFINES
#include <math.h>
#ifndef M_PI
    #define M_PI 3.14159265359
#endif

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "../../util/interpolations.hpp"
#include "../../util/makeUnique.hpp"

const RenderMode DEFAULT_RENDER = RenderMode::U_MODEL_U_COLOR;
const int INIT_PARTICLE_PER_SEC = 100;
const ParticleType INIT_PARTICLE_TYPE = ParticleType::SPHERE;
const ParticleBlend INIT_BLEND_TYPE = ParticleBlend::NONE;
const float INIT_DIR_SPREAD = 20.0f;
const EmitterDirection INIT_EMIT_DIR = EmitterDirection::UNIFORM;
const glm::vec3 INIT_EMITTER_POSN = glm::vec3(0.0f);
const EmitterType INIT_EMITTER_TYPE = EmitterType::POINT;
const int MAX_PARTICLE_NUM = 10000;
const float RANDOMNESS_SCALE = 0.01f;

const glm::vec3 INIT_PARTICLE_COLOR = glm::vec3(1.0f);
const float INIT_COLOR_RANDOMNESS = 0.0f;
const float INIT_PARTICLE_FEATHER = 50.0;
const float INIT_FEATHER_RANDOMNESS = 0.0f;
const float INIT_PARTICLE_LIFE = 3.0f;
const float INIT_LIFE_RANDOMNESS = 0.0f;
const float INIT_PARTICLE_OPACITY = 1.0f;
const float INIT_OPACITY_RANDOMNESS = 0.0f;
const glm::vec3 INIT_PARTICLE_ROTATION = glm::vec3(0.0f);
const float INIT_ROTATION_RANDOMNESS = 0.0f;
const float INIT_PARTICLE_SIZE = 1.0f;
const float INIT_SIZE_RANDOMNESS = 0.0f;
const float INIT_VELOCITY = 100.0f;
const float INIT_VELOCITY_RANDOMNESS = 0.2f;
const float INIT_VELOCITY_RANDOMNESS_DIST = 0.5f;
const float PARTICLE_SIZE_SCALE = 0.2f;
const float PARTICLE_VELOCITY_SCALE = 7.0f / 1000.0f;

Emitter::Emitter(const float& secondPerFrame) :
    /***** Emitter Attributes *****/
    direction(INIT_EMIT_DIR), directionSpread(-1.0),
    emitterType(INIT_EMITTER_TYPE), initVelocity(INIT_VELOCITY * PARTICLE_VELOCITY_SCALE),
    initVelocityRandom(INIT_VELOCITY_RANDOMNESS), initVelocityRandomDistribution(INIT_VELOCITY_RANDOMNESS_DIST),
    particlesPerSec(INIT_PARTICLE_PER_SEC), position(INIT_EMITTER_POSN), rotation(glm::vec3(-1.0f)), randGen(),
    /***** Particle Attributes *****/
    blendType(INIT_BLEND_TYPE), feather(INIT_PARTICLE_FEATHER),
    particleColor(INIT_PARTICLE_COLOR), particleColorRandom(INIT_COLOR_RANDOMNESS),
    particleLife(INIT_PARTICLE_LIFE), particleLifeRandom(INIT_LIFE_RANDOMNESS),
    particleOpacity(INIT_PARTICLE_OPACITY), particleOpacityRandom(INIT_OPACITY_RANDOMNESS),
    particleRotation(INIT_PARTICLE_ROTATION), particleRotationRandom(INIT_ROTATION_RANDOMNESS),
    particleSize(INIT_PARTICLE_SIZE), particleSizeRandom(INIT_SIZE_RANDOMNESS),
    particlesPerFrame(INIT_PARTICLE_PER_SEC * secondPerFrame), particleType(INIT_PARTICLE_TYPE),
    /***** User Unmodifiables *****/
    secondPerFrame(secondPerFrame), emitterRenderMode(DEFAULT_RENDER), lastUsedParticle(0) {

    // Initialize geometry data
    setGeometry(particleType);
    curGeomtry->init();

    // Reserve memory for particles
    particles.reserve(MAX_PARTICLE_NUM);

    // Reserve memory for instanced particle info
    offsets.reserve(MAX_PARTICLE_NUM * 3);
    colors.reserve(MAX_PARTICLE_NUM * 3);
    modelMatrices.reserve(MAX_PARTICLE_NUM * 16);
}

Emitter::~Emitter() {}

/***** Emitter Attributes *****/
const glm::vec3& Emitter::getEmitterPosn() const {
    return position;
}

float* Emitter::getEmitterPosnPtr() {
    return glm::value_ptr(position);
}



/***** Particle Attributes *****/
size_t Emitter::getParticleNum() const {
    return particles.size();
}

uint32_t* Emitter::getParticlesPerSecPtr() {
    return &particlesPerSec;
}

ParticleType Emitter::getParticleType() const {
    return particleType;
}

void Emitter::setParticleType(ParticleType particleType) {
    // Run only if particle type has changed
    if (particleType != this->particleType) {
        this->particleType = particleType;
        setGeometry(particleType);
        curGeomtry->init();
    }
}

const glm::vec3& Emitter::getParticleColor() const {
    return particleColor;
}

float* Emitter::getParticleColorPtr() {
    return glm::value_ptr(particleColor);
}

float* Emitter::getParticleColorRandomnessPtr() {
    return &particleColorRandom;
}

float* Emitter::getParticleLifePtr() {
    return &particleLife;
}

float* Emitter::getParticleLifeRandomnessPtr() {
    return &particleLifeRandom;
}

float Emitter::getParticleRotationRandomness() const {
    return particleRotationRandom;
}

float Emitter::getParticleSize() const {
    return particleSize;
}

float* Emitter::getParticleSizePtr() {
    return &particleSize;
}

float* Emitter::getParticleSizeRandomnessPtr() {
    return &particleSizeRandom;
}

float* Emitter::getInitialVelocityPtr() {
    return &initVelocity;
}

float* Emitter::getInitialVelocityRandomnessPtr() {
    return &initVelocityRandom;
}

float* Emitter::getInitialVelocityRandomnessDistributionPtr() {
    return &initVelocityRandomDistribution;
}

float Emitter::getBaseScale() const {
    return curGeomtry->getBaseScale();
}

GLenum Emitter::getDrawMode() const {
    return curGeomtry->getDrawMode();
}

int Emitter::getIndexNum() const{
    return curGeomtry->getIndexNum();
}

int Emitter::getCurrentParticleNum() const {
    return activeParticleNum;
}

const std::vector<float>& Emitter::getInstancedColors() const {
    return colors;
}

const std::vector<float>& Emitter::getOffsets() const {
    return offsets;
}

const std::vector<float>& Emitter::getModelMatrices() const{
    return modelMatrices;
}

RenderMode Emitter::getRenderMode() const {
    return emitterRenderMode;
}

uint32_t Emitter::getVAO() const {
    return curGeomtry->getVAO();
}

bool Emitter::useEBO() const {
    return curGeomtry->useEBO();
}

void Emitter::update(const float& interpolation) {
    updateRenderMode();
    activeParticleNum = 0;

    // Update particlesPerFrame with (possibly) new particle emit rate and framerate
    particlesPerFrame = particlesPerSec * secondPerFrame;

    // Get the number of new particles to render during this time period
    float newParticleNum;
    newParticleNumBuffer += particlesPerFrame * interpolation;
    newParticleNumBuffer = std::modf(newParticleNumBuffer, &newParticleNum);

    // First, add new particles generated in this time
    int newParticleIndex = 0;
    for (int i = 0; i < newParticleNum; ++i) {
        newParticleIndex = getFirstUnusedParticle();

        float newParticleInitVelocity = initVelocity;
        // Calculate random velocity if randomness is set
        if (initVelocityRandom > 0.0f) {
            float offset = (randGen.randBool(initVelocityRandomDistribution) ? 1 : -1) * randGen.randRealOpenRight(0.0f, initVelocityRandom);
            newParticleInitVelocity = initVelocity + offset;
        }

        // Calculate actual velocity on different axis using spherical coordinate
        float inclination = randGen.randRealClosed(0.0f, M_PI);
        float azimuth = randGen.randRealOpenRight(0.0, 2 * M_PI);
        float horizontalVelocity = newParticleInitVelocity * sinf(inclination);
        glm::vec3 newParticleVelocity(horizontalVelocity * sinf(azimuth), newParticleInitVelocity * cosf(inclination), horizontalVelocity * cosf(azimuth));

        glm::vec3 newParticleColor(particleColor);
        // Calculate random color if randomness is set
        if (particleColorRandom > 0.0f) {
           glm::vec3 randomColor = randGen.randVec3(0.0f, 1.0f);
           newParticleColor = colorLerp(randomColor, particleColor, particleColorRandom);
        }

        float newParticleLife = particleLife;
        // Calculate random life if randomness is set
        if (particleLifeRandom > 0.0f) {
            float offset = particleLifeRandom * particleLife;
            newParticleLife = fmax(randGen.randRealOpenRight(particleLife - offset, particleLife + offset), 0.0f);
        }

        float newParticleSize = particleSize;
        // Calculate random size if randomness is set
        if (particleSizeRandom > 0.0f) {
            float offset = particleSizeRandom * particleSize;
            newParticleSize = fmax(randGen.randRealClosed(particleSize - offset, particleSize + offset), 0.0f);
        }

        if (newParticleIndex < 0) {
            // Push new particles to the particles vector if all particles are in use
            particles.emplace_back(newParticleVelocity, newParticleColor, newParticleLife, newParticleSize);
        } else {
            // Otherwise replace the unused particle
            particles[newParticleIndex] = Particle(newParticleVelocity, newParticleColor, newParticleLife, newParticleSize);
        }
    }

    // Then update particle attributes
    colors.clear();
    modelMatrices.clear();
    offsets.clear();

    for (auto& particle : particles) {
        if (particle.life <= 0) continue;

        // Add to current particle to active particles
        activeParticleNum++;

        // Update particle info
        particle.life -= secondPerFrame * interpolation;
        particle.offset += particle.velocity * interpolation;

        // Update particle offset if render mode is Uniform Model
        if (emitterRenderMode == RenderMode::U_MODEL_U_COLOR || emitterRenderMode == RenderMode::U_MODEL_V_COLOR) {
            offsets.emplace_back(particle.offset.x);
            offsets.emplace_back(particle.offset.y);
            offsets.emplace_back(particle.offset.z);
        // Else update particle model matrices
        } else {
            glm::mat4 model;
            model = glm::translate(model, particle.offset);
            if (particle.rotation != glm::vec3(0.0f)) {
                model = glm::rotate(model, particle.rotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
                model = glm::rotate(model, particle.rotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
                model = glm::rotate(model, particle.rotation.z, glm::vec3(0.0f, 0.0f, 1.0f));
            }
            model = glm::scale(model, glm::vec3(particle.size));
            float* modelArr = glm::value_ptr(model);
            modelMatrices.insert(modelMatrices.end(), modelArr, modelArr + 16);
        }

        if (emitterRenderMode == RenderMode::V_MODEL_V_COLOR || emitterRenderMode == RenderMode::U_MODEL_V_COLOR) {
            colors.emplace_back(particle.color.r);
            colors.emplace_back(particle.color.g);
            colors.emplace_back(particle.color.b);
        }
    }
}

/***** Private *****/

int Emitter::getFirstUnusedParticle() {
    for (uint16_t i = lastUsedParticle; i < particles.size(); ++i) {
        if (particles[i].life <= 0) {
            lastUsedParticle = i;
            return i;
        }
    }

    for (uint16_t j = 0; j < lastUsedParticle; ++j) {
        if (particles[j].life <= 0) {
            lastUsedParticle = j;
            return j;
        }
    }

    // if all particles are in use...
    if (particles.size() < particles.capacity()) {
        // if the number of particles have not reached MAX_PARTICLE_NUM yet, create a new particle
        lastUsedParticle = particles.size();
        return -1;
    } else {
        // else override the first particle
        lastUsedParticle = 0;
        return 0;
    }
}

void Emitter::setGeometry(ParticleType particleType) {
    switch (particleType) {
        case ParticleType::SPHERE:
            curGeomtry = make_unique<Sphere>();
            break;
        case ParticleType::SQUARE:
            curGeomtry = make_unique<Square>();
            break;
        case ParticleType::TRIANGLE:
            curGeomtry = make_unique<Triangle>();
            break;
        default:
            break;
    };
}

void Emitter::updateRenderMode() {
    bool colorVaring = false;
    bool modelVaring = false;

    if (particleColorRandom > 0.0f) {
        colorVaring = true;
    }

    if (particleRotationRandom + particleSizeRandom > 0.0f) {
        modelVaring = true;
    }

    if (colorVaring && modelVaring) {
        emitterRenderMode = RenderMode::V_MODEL_V_COLOR;
    } else if (colorVaring) {
        emitterRenderMode = RenderMode::U_MODEL_V_COLOR;
    } else if (modelVaring) {
        emitterRenderMode = RenderMode::V_MODEL_U_COLOR;
    } else {
        emitterRenderMode = RenderMode::U_MODEL_U_COLOR;
    }
}
