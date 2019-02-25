#include "emitter.hpp"

Emitter::Emitter(ParticleType particleType, uint32_t particlesPerSec) : particleType(particleType), particlesPerSec(particlesPerSec) {
    switch (particleType) {
        case ParticleType::SPHERE:
            curGeomtry = std::unique_ptr<Sphere>(new Sphere());
            break;
        case ParticleType::SQUARE:
            // curGeomtry = std::unique_ptr<Square>(new Square());
            break;
        case ParticleType::TRIANGLE:
            // curGeomtry = std::unique_ptr<Triangle>(new Triangle());
            break;
        default:
            break;
    };

    curGeomtry->init();
}

Emitter::~Emitter() {}

glm::vec3* Emitter::getOffsets() {
    offsets.reserve(100);

    /***** Use Random if necessary afterwards *****/
    for (uint32_t i = 0; i < 100; ++i) {
        offsets.emplace_back(glm::vec3(0 * 1.0f, i * 1.0f, 0.0));
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
