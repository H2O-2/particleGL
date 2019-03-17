#include "emitter.hpp"

const int INIT_PARTICLE_PER_SEC = 100;
const ParticleBlend INIT_BLEND_TYPE = ParticleBlend::NONE;
const ParticleType INIT_PARTICLE_TYPE = ParticleType::SPHERE;
const EmitterDirection INIT_EMIT_DIR = EmitterDirection::UNIFORM;
const float INIT_DIR_SPREAD = 20.0f;
const EmitterType INIT_EMITTER_TYPE = EmitterType::POINT;
const glm::vec3 INIT_EMITTER_POSN = glm::vec3(0.0f);
const glm::vec3 INIT_PARTICLE_ROTATION = glm::vec3(0.0f);
const glm::vec3 INIT_PARTICLE_SIZE = glm::vec3(5.0f, 5.0f, 1.0f);
const float INIT_FEATHER = 50.0;
const float INIT_VELOCITY = 1.0f;
const float INIT_LIFE = 3.0f;

Emitter::Emitter(const float& secondPerFrame) : newParticleType(INIT_PARTICLE_TYPE), particleAmount(50), blendType(INIT_BLEND_TYPE), // DEBUG
        particlesPerSec(INIT_PARTICLE_PER_SEC), particleType(INIT_PARTICLE_TYPE), direction(INIT_EMIT_DIR), directionSpread(-1.0),
        emitterType(INIT_EMITTER_TYPE), position(INIT_EMITTER_POSN), rotation(glm::vec3(-1.0f)), size(INIT_PARTICLE_SIZE),
        feather(0.0f), initVelocity(INIT_VELOCITY), particleLife(INIT_LIFE), lifeRandom(0), opacityRandom(0), rotationRandom(0),
        sizeRandom(0), secondPerFrame(secondPerFrame) {

    // Initialize geometry data
    setGeometry(particleType);
    curGeomtry->init();

    // Reserve memory for particles
    particles.reserve((size_t)particlesPerSec * particleLife);

    /***** DEBUG *****/
    for (int i = 0; i < particleAmount; ++i) {
        bool randomMinus = randGen.randBool();
        float velocityX = randGen.randRealClosed(-initVelocity, initVelocity);
        particles.emplace_back(glm::vec3(velocityX, (randomMinus ? -1.0f : 1.0f) * glm::sqrt(initVelocity * initVelocity - velocityX * velocityX), 0.0f));
    }
    /***** DEBUG *****/
}

Emitter::~Emitter() {}

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
    std::vector<float> offsets;
    offsets.reserve(particles.size() * 3);

    for (uint32_t i = 0; i < particles.size(); ++i) {
        offsets.emplace_back(particles[i].offset.x);
        offsets.emplace_back(particles[i].offset.y);
        offsets.emplace_back(particles[i].offset.z);
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
    for (auto& particle : particles) {
        particle.age += secondPerFrame * interpolation;
        particle.offset += particle.velocity * interpolation;
    }
}

/***** Private *****/

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
