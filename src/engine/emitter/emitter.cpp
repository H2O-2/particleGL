#include "emitter.hpp"

Emitter::Emitter(ParticleType particleType, uint32_t particlesPerSec) : particleType(particleType), particlesPerSec(particlesPerSec) {
    switch (particleType) {
        case ParticleType::SPHERE:
            curGeomtry = std::unique_ptr<Sphere>(new Sphere());
            break;
        case ParticleType::SQUARE:
            curGeomtry = std::unique_ptr<Square>(new Square());
            break;
        case ParticleType::TRIANGLE:
            curGeomtry = std::unique_ptr<Triangle>(new Triangle());
            break;
        default:
            break;
    };
}

Emitter::~Emitter() {}

int Emitter::getindexNum() {
    return curGeomtry->getIndexNum();
}

void Emitter::draw() {

}
