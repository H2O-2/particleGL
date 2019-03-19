#include "emitter.hpp"

const int MAX_PARTICLE_NUM = 10000;
const int INIT_PARTICLE_PER_SEC = 100;
const ParticleBlend INIT_BLEND_TYPE = ParticleBlend::NONE;
const ParticleType INIT_PARTICLE_TYPE = ParticleType::SPHERE;
const EmitterDirection INIT_EMIT_DIR = EmitterDirection::UNIFORM;
const float INIT_DIR_SPREAD = 20.0f;
const EmitterType INIT_EMITTER_TYPE = EmitterType::POINT;
const glm::vec3 INIT_EMITTER_POSN = glm::vec3(0.0f);
const glm::vec3 INIT_PARTICLE_ROTATION = glm::vec3(0.0f);
const glm::vec3 INIT_PARTICLE_SIZE = glm::vec3(2.0f, 2.0f, 1.0f);
const float INIT_FEATHER = 50.0;
const float INIT_VELOCITY = 0.3f;
const float INIT_LIFE = 3.0f;

Emitter::Emitter(const float& secondPerFrame) : newParticleType(INIT_PARTICLE_TYPE), secondPerFrame(secondPerFrame),
        blendType(INIT_BLEND_TYPE), particlesPerSec(INIT_PARTICLE_PER_SEC),
        particlesPerFrame(INIT_PARTICLE_PER_SEC * secondPerFrame), particleType(INIT_PARTICLE_TYPE),
        direction(INIT_EMIT_DIR), directionSpread(-1.0), emitterType(INIT_EMITTER_TYPE), position(INIT_EMITTER_POSN),
        rotation(glm::vec3(-1.0f)), feather(0.0f), initVelocity(INIT_VELOCITY), particleLife(INIT_LIFE), lifeRandom(0),
        opacityRandom(0), rotationRandom(0), sizeRandom(0), lastUsedParticle(0) {

    // Initialize geometry data
    setGeometry(particleType);
    curGeomtry->init();

    // Reserve memory for particles
    particles.reserve(MAX_PARTICLE_NUM);

    // Reserve memory for particle offsets
    offsets.reserve(MAX_PARTICLE_NUM * 3);
}

Emitter::~Emitter() {}

size_t Emitter::getParticleNum() const {
    return particles.size();
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

float Emitter::getBaseScale() const {
    return curGeomtry->getBaseScale();
}

GLenum Emitter::getDrawMode() const {
    return curGeomtry->getDrawMode();
}

int Emitter::getIndexNum() const{
    return curGeomtry->getIndexNum();
}

std::vector<float> Emitter::getOffsets() {
    for (size_t i = 0; i < particles.size(); i++) {
        Particle particle = particles[i];
        if (particle.life > 0) {
            if (offsets.size() > i * 3) {
                offsets[i * 3] = particle.offset.x;
                offsets[i * 3 + 1] = particle.offset.y;
                offsets[i * 3 + 2] = particle.offset.z;
            } else {
                offsets.emplace_back(particle.offset.x);
                offsets.emplace_back(particle.offset.y);
                offsets.emplace_back(particle.offset.z);
            }
        }
    }

    return offsets;
}

glm::mat4* Emitter::getModelMatrices() const{
    std::vector<glm::mat4> modelMatrices;

    /***** Use Random if necessary afterwards *****/
    // position


    return modelMatrices.data();
}

uint32_t Emitter::getVAO() const {
    return curGeomtry->getVAO();
}

bool Emitter::useEBO() const {
    return curGeomtry->useEBO();
}

void Emitter::update(const float& interpolation) {
    // First, add particles generated in one frame

    /***** TODO: Fix the case where particlesPerFrame is smaller than 1 *****/

    uint32_t newParticleNum = particlesPerFrame * interpolation;
    int newParticleIndex = 0;
    for (uint32_t i = 0; i < newParticleNum; ++i) {
        bool randomMinus = randGen.randBool();
        float velocityX = randGen.randRealClosed(-initVelocity, initVelocity);
        newParticleIndex = getFirstUnusedParticle();
        glm::vec3 newParticleVelocity = glm::vec3(velocityX, (randomMinus ? -1.0f : 1.0f) * glm::sqrt(initVelocity * initVelocity - velocityX * velocityX), 0.0f);
        if (newParticleIndex < 0) {
            particles.emplace_back(newParticleVelocity);
        } else {
            particles[newParticleIndex] = Particle(newParticleVelocity);
        }
    }

    // Update particle info
    for (auto& particle : particles) {
        if (particle.life > 0) {
            particle.life -= secondPerFrame * interpolation;
            particle.offset += particle.velocity * interpolation;
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
