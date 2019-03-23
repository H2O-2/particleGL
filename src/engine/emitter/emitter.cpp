#include "emitter.hpp"

#define _USE_MATH_DEFINES
#include <math.h>

#include <glm/gtc/type_ptr.hpp>
#include "../../util/interpolations.hpp"
#include "../../util/makeUnique.hpp"

const int MAX_PARTICLE_NUM = 10000;
const int INIT_PARTICLE_PER_SEC = 100;
const ParticleBlend INIT_BLEND_TYPE = ParticleBlend::NONE;
const ParticleType INIT_PARTICLE_TYPE = ParticleType::SPHERE;
const EmitterDirection INIT_EMIT_DIR = EmitterDirection::UNIFORM;
const float INIT_DIR_SPREAD = 20.0f;
const EmitterType INIT_EMITTER_TYPE = EmitterType::POINT;
const glm::vec3 INIT_EMITTER_POSN = glm::vec3(0.0f);
const RenderMode DEFAULT_RENDER = RenderMode::U_MODEL_U_COLOR;
const float RANDOMNESS_SCALE = 0.01f;

const glm::vec3 INIT_PARTICLE_COLOR = glm::vec3(1.0f);
const float INIT_COLOR_RANDOMNESS = 0.0f;

const glm::vec3 INIT_PARTICLE_ROTATION = glm::vec3(0.0f);
const float INIT_ROTATION_RANDOMNESS = 0.0f;

const float INIT_PARTICLE_SIZE = 1.0f;
const float PARTICLE_SIZE_SCALE = 0.2f;
const float INIT_FEATHER = 50.0;
const float PARTICLE_VELOCITY_SCALE = 7.0f / 1000.0f;
const float INIT_VELOCITY = 100.0f;
const float INIT_LIFE = 3.0f;

Emitter::Emitter(const float& secondPerFrame) : newParticleType(INIT_PARTICLE_TYPE), secondPerFrame(secondPerFrame),
        blendType(INIT_BLEND_TYPE), particlesPerSec(INIT_PARTICLE_PER_SEC),
        particlesPerFrame(INIT_PARTICLE_PER_SEC * secondPerFrame), particleType(INIT_PARTICLE_TYPE),
        direction(INIT_EMIT_DIR), directionSpread(-1.0), emitterType(INIT_EMITTER_TYPE), position(INIT_EMITTER_POSN),
        rotation(glm::vec3(-1.0f)), particleColor(INIT_PARTICLE_COLOR), particleColorRandom(INIT_COLOR_RANDOMNESS),
        feather(0.0f), initVelocity(INIT_VELOCITY * PARTICLE_VELOCITY_SCALE), particleLife(INIT_LIFE), particleLifeRandom(0),
        particleOpacityRandom(0), particleRotationRandom(INIT_ROTATION_RANDOMNESS), particleSize(INIT_PARTICLE_SIZE), particleSizeRandom(0),
        emitterRenderMode(DEFAULT_RENDER), lastUsedParticle(0) {

    // Initialize geometry data
    setGeometry(particleType);
    curGeomtry->init();

    // Reserve memory for particles
    particles.reserve(MAX_PARTICLE_NUM);

    // Reserve memory for instanced particle info
    offsets.reserve(MAX_PARTICLE_NUM * 3);
    colors.reserve(MAX_PARTICLE_NUM * 3);
}

Emitter::~Emitter() {}

size_t Emitter::getParticleNum() const {
    return particles.size();
}

uint32_t* Emitter::getParticlesPerSecPtr() {
    return &particlesPerSec;
}

float* Emitter::getParticleLifePtr() {
    return &particleLife;
}

float Emitter::getParticleSize() const {
    return particleSize;
}

float* Emitter::getParticleSizePtr() {
    return &particleSize;
}

float Emitter::getParticleSizeRandomness() const {
    return particleSizeRandom;
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

glm::vec3 Emitter::getParticleColor() const {
    return particleColor;
}

float* Emitter::getParticleColorPtr() {
    return glm::value_ptr(particleColor);
}

float Emitter::getParticleColorRandomness() const {
    return particleColorRandom;
}

float* Emitter::getParticleColorRandomnessPtr() {
    return &particleColorRandom;
}

float* Emitter::getInitialVelocityPtr() {
    return &initVelocity;
}

float Emitter::getParticleRotationRandomness() const {
    return particleRotationRandom;
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

std::vector<float> Emitter::getInstancedColors() const {
    return colors;
}

std::vector<float> Emitter::getOffsets() const {
    return offsets;
}

glm::mat4* Emitter::getModelMatrices() const{
    std::vector<glm::mat4> modelMatrices;

    /***** Use Random if necessary afterwards *****/
    // position


    return modelMatrices.data();
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

        float inclination = randGen.randRealClosed(0.0f, M_PI);
        float azimuth = randGen.randRealOpenRight(0.0, 2 * M_PI);
        float horizontalVelocity = initVelocity * sinf(inclination);
        glm::vec3 newParticleVelocity(horizontalVelocity * sinf(azimuth), initVelocity * cosf(inclination), horizontalVelocity * cosf(azimuth));

        glm::vec3 newParticleColor(particleColor);
        if (particleColorRandom > 0.0f) {
           glm::vec3 randomColor = randGen.randVec3(0.0f, 1.0f);
           newParticleColor = colorLerp(randomColor, particleColor, particleColorRandom);
        }

        if (newParticleIndex < 0) {
            particles.emplace_back(newParticleVelocity, newParticleColor, particleLife, particleSize);
        } else {
            particles[newParticleIndex] = Particle(newParticleVelocity, newParticleColor, particleLife, particleSize);
        }
    }

    // Then update particle attributes
    colors.clear();
    offsets.clear();

    for (size_t j = 0; j < particles.size(); ++j) {
        Particle& particle = particles[j];

        if (particle.life > 0) {
            // Update particle info
            particle.life -= secondPerFrame * interpolation;
            particle.offset += particle.velocity * interpolation;

            // Update particle offset if render mode is Uniform Model
            if (emitterRenderMode == RenderMode::U_MODEL_U_COLOR || emitterRenderMode == RenderMode::U_MODEL_V_COLOR) {
                if (offsets.size() > j * 3) {
                    offsets[j * 3] = particle.offset.x;
                    offsets[j * 3 + 1] = particle.offset.y;
                    offsets[j * 3 + 2] = particle.offset.z;
                } else {
                    offsets.emplace_back(particle.offset.x);
                    offsets.emplace_back(particle.offset.y);
                    offsets.emplace_back(particle.offset.z);
                }
            }

            if (emitterRenderMode == RenderMode::V_MODEL_V_COLOR || emitterRenderMode == RenderMode::U_MODEL_V_COLOR) {
                if (colors.size() > j * 3) {
                    colors[j * 3] = particle.color.r;
                    colors[j * 3 + 1] = particle.color.g;
                    colors[j * 3 + 2] = particle.color.b;
                } else {
                    colors.emplace_back(particle.color.r);
                    colors.emplace_back(particle.color.g);
                    colors.emplace_back(particle.color.b);
                }
            }
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
