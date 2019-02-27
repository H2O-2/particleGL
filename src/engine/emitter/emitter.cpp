#include "emitter.hpp"

Emitter::Emitter(ParticleType particleType, uint32_t particlesPerSec) : particleType(particleType), particlesPerSec(particlesPerSec) {
    switch (particleType) {
        case ParticleType::SPHERE:
            curGeomtry = make_unique<Sphere>();
            break;
        case ParticleType::SQUARE:
            // curGeomtry = make_unique<Square>();
            break;
        case ParticleType::TRIANGLE:
            // curGeomtry = make_unique<Triangle>();
            break;
        default:
            break;
    };

    curGeomtry->init();

    /***** DEBUG *****/
    particleAmount = 100;
    for (int i = 0; i < particleAmount; ++i) {
        particles.emplace_back();
}
    /***** DEBUG *****/
}

Emitter::~Emitter() {}

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

void Emitter::draw() {

}
