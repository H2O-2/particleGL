#include "emitter.hpp"

#include <algorithm>
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
const float INIT_DIR_SPREAD = 0.2f;
const EmitterDirection INIT_EMIT_DIR = EmitterDirection::UNIFORM;
const glm::vec3 INIT_EMITTER_POSN = glm::vec3(0.0f);
const glm::vec3 INIT_EMITTER_ROTATION = glm::vec3(0.0f);
const glm::vec3 INIT_EMITTER_SIZE = glm::vec3(105.0f);
const float EMITTER_SIZE_SCALE = 0.21f;
const EmitterSize INIT_EMITTER_SIZE_TYPE = EmitterSize::LINKED;
const EmitterType INIT_EMITTER_TYPE = EmitterType::POINT;
const int MAX_PARTICLE_NUM = 30000;
const float PERCENTAGE_SCALE = 0.01f;

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
const float INIT_PARTICLE_ASPECT = 1.0f;
const float INIT_PARTICLE_SIZE = 1.0f;
const float INIT_SIZE_RANDOMNESS = 0.0f;
const float INIT_VELOCITY = 100.0f;
const float INIT_VELOCITY_RANDOMNESS = 0.2f;
const float INIT_VELOCITY_RANDOMNESS_DIST = 0.5f;
const float PARTICLE_SIZE_SCALE = 0.2f;
const float PARTICLE_VELOCITY_SCALE = 0.007f;

Emitter::Emitter(const float& secondPerFrame) :
    /***** Emitter Attributes *****/
    direction(INIT_EMIT_DIR), directionSpread(INIT_DIR_SPREAD),
    emitterType(INIT_EMITTER_TYPE), initVelocity(INIT_VELOCITY * PARTICLE_VELOCITY_SCALE),
    initVelocityRandom(INIT_VELOCITY_RANDOMNESS), initVelocityRandomDistribution(INIT_VELOCITY_RANDOMNESS_DIST),
    particlesPerSec(INIT_PARTICLE_PER_SEC), position(INIT_EMITTER_POSN), rotation(INIT_EMITTER_ROTATION),
    size(INIT_EMITTER_SIZE), emitterSizeType(INIT_EMITTER_SIZE_TYPE), randGen(),
    /***** Particle Attributes *****/
    feather(INIT_PARTICLE_FEATHER), particleAspectRatio(INIT_PARTICLE_SIZE),
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
EmitterDirection Emitter::getEmitterDirectionType() {
    return direction;
}

EmitterDirection* Emitter::getEmitterDirectionTypePtr() {
    return &direction;
}

float* Emitter::getEmitterDirectionSpreadPtr() {
    return &directionSpread;
}

const glm::vec3& Emitter::getEmitterPosn() const {
    return position;
}

float* Emitter::getEmitterPosnPtr() {
    return glm::value_ptr(position);
}

float* Emitter::getEmitterRotationPtr() {
    return glm::value_ptr(rotation);
}

float* Emitter::getEmitterSizePtr() {
    return glm::value_ptr(size);
}

EmitterSize Emitter::getEmitterSizeType() const {
    return emitterSizeType;
}

EmitterSize* Emitter::getEmitterSizeTypePtr() {
    return &emitterSizeType;
}

EmitterType Emitter::getEmitterType() const {
    return emitterType;
}

EmitterType* Emitter::getEmitterTypePtr() {
    return &emitterType;
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

float* Emitter::getParticleAspectRatioPtr() {
    return &particleAspectRatio;
}

const glm::vec4 Emitter::getParticleColorAndOpacity() const {
    return glm::vec4(particleColor, particleOpacity);
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

float* Emitter::getParicleOpacityPtr() {
    return &particleOpacity;
}

float* Emitter::getParicleOpacityRandomnessPtr() {
    return &particleOpacityRandom;
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

void Emitter::updateCurGeometry() {
    switch (particleType) {
        case ParticleType::SQUARE:
            static_cast<Square*>(curGeomtry.get())->setAspectRatio(particleAspectRatio);
            break;
        default:
            break;
    }
}

void Emitter::update(const float& interpolation) {
    updateRenderMode();

    updateCurGeometry();

    // Update emitter's model matrix
    emitterRotation = glm::rotate(glm::mat4(), glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
    emitterRotation = glm::rotate(emitterRotation, glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
    emitterRotation = glm::rotate(emitterRotation, glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
    emitterTranslation = glm::translate(glm::mat4(), position);

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

        glm::vec3 newParticleColor(particleColor);
        // Calculate random color if randomness is set
        if (particleColorRandom > 0.0f) {
           glm::vec3 randomColor = randGen.randVec3Closed(0.0f, 1.0f);
           newParticleColor = colorLerp(randomColor, particleColor, particleColorRandom);
        }

        float newParticleLife = particleLife;
        // Calculate random life if randomness is set
        if (particleLifeRandom > 0.0f) {
            float offset = particleLifeRandom * particleLife;
            newParticleLife = fmax(randGen.randRealOpenRight(particleLife - offset, particleLife + offset), 0.0f);
        }

        float newParticleOpacity = particleOpacity;
        // Calculate random opacity if randomness is set
        if (particleOpacityRandom > 0.0f) {
            float offset = particleOpacityRandom * particleOpacity;
            newParticleOpacity = glm::clamp(randGen.randRealClosed(particleOpacity - offset, particleOpacity + offset), 0.0f, 1.0f);
        }

        float newParticleSize = particleSize;
        // Calculate random size if randomness is set
        if (particleSizeRandom > 0.0f) {
            float offset = particleSizeRandom * particleSize;
            newParticleSize = fmax(randGen.randRealClosed(particleSize - offset, particleSize + offset), 0.0f);
        }

        if (newParticleIndex < 0) {
            // Push new particles to the particles vector if all particles are in use
            particles.emplace_back(newParticleLife, newParticleColor, generateInitialParticlePosn(), newParticleOpacity, newParticleSize, generateInitialParticleVelocity());
        } else {
            // Otherwise replace the unused particle
            particles[newParticleIndex] = Particle(newParticleLife, newParticleColor, generateInitialParticlePosn(), newParticleOpacity, newParticleSize, generateInitialParticleVelocity());
        }
    }

    // Then update particle attributes
    colors.clear();
    modelMatrices.clear();
    offsets.clear();

    // Sort particles from far to near the camera for blending to work properly
    std::sort(particles.begin(), particles.end());

    activeParticleNum = 0;

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
            colors.emplace_back(particle.opacity);
        }
    }
}

/***** Private *****/

glm::vec3 Emitter::generateInitialParticlePosn() {
    glm::mat4 emitterModel = emitterTranslation * emitterRotation;
    glm::vec3 halfSize = size / 2.0f;
    switch (emitterType) {
        case EmitterType::BOX:
            return glm::vec3(emitterModel * glm::vec4(randGen.randVec3Closed(-halfSize, halfSize), 1.0f));
        case EmitterType::SPHERE:
            // TODO
            return glm::vec3();
        default:
            return glm::vec3(emitterModel * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
    }
}

glm::vec3 Emitter::generateInitialParticleVelocity() {
    float newParticleInitVelocity = initVelocity;
    // Calculate random velocity if randomness is set
    if (initVelocityRandom > 0.0f) {
        float offset = (randGen.randBool(initVelocityRandomDistribution) ? 1 : -1) * randGen.randRealOpenRight(0.0f, initVelocityRandom);
        newParticleInitVelocity = fmax(initVelocity + offset, 0.0f);
    }

    // Calculate actual velocity on different axis using spherical coordinate
    float inclination, azimuth;
    if (direction == EmitterDirection::UNIFORM) {
        inclination = randGen.randRealClosed(0.0f, M_PI);
        azimuth = randGen.randRealOpenRight(0.0, 2 * M_PI);
    } else if (direction == EmitterDirection::DIRECTIONAL) {
        float halfPi = M_PI / 2.0f;
        float halfSpread = directionSpread * M_PI;
        float quaterSpread = halfSpread / 2.0f;
        inclination = randGen.randRealClosed(halfPi - quaterSpread, halfPi + quaterSpread);
        azimuth = randGen.randRealOpenRight(-halfSpread, halfSpread);
    }

    float horizontalVelocity = newParticleInitVelocity * sinf(inclination);

    // Apply only emitter's rotation matrix to velocity since vector does not have position
    return glm::vec3(emitterRotation * glm::vec4(horizontalVelocity * sinf(azimuth), newParticleInitVelocity * cosf(inclination), horizontalVelocity * cosf(azimuth), 1.0f));
}

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

    if (particleColorRandom + particleOpacityRandom > 0.0f) {
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
