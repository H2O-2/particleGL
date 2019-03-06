#include "emitter.hpp"

const int INIT_PARTICLE_PER_SEC = 100;
const ParticleBlend INIT_BLEND_TYPE = ParticleBlend::NONE;
const ParticleType INIT_PARTICLE_TYPE = ParticleType::SPHERE;
const EmitterDirection INIT_EMIT_DIR = EmitterDirection::UNIFORM;
const float INIT_DIR_SPREAD = 20.0f;
const EmitterType INIT_EMITTER_TYPE = EmitterType::POINT;
const glm::vec3 INIT_PARTICLE_POSN = glm::vec3(0.0f);
const glm::vec3 INIT_PARTICLE_ROTATION = glm::vec3(0.0f);
const glm::vec3 INIT_PARTICLE_SIZE = glm::vec3(5.0f, 5.0f, 1.0f);
const float INIT_FEATHER = 50.0;
const float INIT_VELOCITY = 100.0f;
const float INIT_LIFE = 3.0f;

Emitter::Emitter(const float& secondPerFrame) : particleAmount(0), blendType(INIT_BLEND_TYPE), // DEBUG
        particlesPerSec(INIT_PARTICLE_PER_SEC), particleType(INIT_PARTICLE_TYPE), direction(INIT_EMIT_DIR), directionSpread(-1.0),
        emitterType(INIT_EMITTER_TYPE), position(INIT_PARTICLE_POSN), rotation(glm::vec3(-1.0f)), size(INIT_PARTICLE_SIZE),
        feather(0.0f), initVelocity(0), particleLife(INIT_LIFE), lifeRandom(0), opacityRandom(0), rotationRandom(0),
        sizeRandom(0), secondPerFrame(secondPerFrame) {

    // Initialize geometry data
    setGeometry(particleType);
    curGeomtry->init();

    // Reserve memory for particles
    particles.reserve((size_t)particlesPerSec * particleLife);

    /***** DEBUG *****/
    particleAmount = 100;
    for (int i = 0; i < particleAmount; ++i) {
        particles.emplace_back();
    }
    /***** DEBUG *****/
}

Emitter::~Emitter() {}

void Emitter::setParticleType(ParticleType particleType) {
    // Run only if particle type has changed
    if (particleType != this->particleType) {
        this->particleType = particleType;
        setGeometry(particleType);
        curGeomtry->init();
    }
}

glm::vec3* Emitter::getOffsets() {
    offsets.reserve(particleAmount);

    /***** Use Random if necessary afterwards *****/
    for (uint32_t i = 0; i < particleAmount; ++i) {
        offsets.emplace_back(0 * 1.0f, i * 1.0f, 0.0);
    }

    return offsets.data();
}

glm::mat4* Emitter::getModelMatrices() {
    std::vector<glm::mat4> modelMatrices;

    /***** Use Random if necessary afterwards *****/
    // position


    return modelMatrices.data();
}

int Emitter::getIndexNum() {
    return curGeomtry->getIndexNum();
}

uint32_t Emitter::getVAO() {
    return curGeomtry->getVAO();
}

void Emitter::update(const float& interpolation) {
    for (auto& particle : particles) {
        particle.age += secondPerFrame;
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
