#include "randGen.hpp"

RandGen::RandGen(uint32_t seed) : seed(seed), engine(RandEngine(seed)), intDist(IntDist(0, 100)), realDist(RealDist(0.0f, 10.0f)) {}

uint32_t RandGen::getSeed() {
    return seed;
}

void RandGen::setSeed(uint32_t seed) {
    this->seed = seed;
    engine.seed(seed);
}

IntDist::result_type RandGen::randInt(const int& rangeMin, const int& rangeMax) {
    // If requested range is available, generate directly
    if (rangeMin == intDist.min() && rangeMax == intDist.max()) {
        return intDist(engine);
    }

    // else re-initialize a new range and generate
    intDist = IntDist(rangeMin, rangeMax);
    return intDist(engine);
}

RealDist::result_type RandGen::randReal(const float& rangeMin, const float& rangeMax) {
    // If requested range is available, generate directly
    if (rangeMin == realDist.min() && rangeMax == realDist.max()) {
        return realDist(engine);
    }

    // else re-initialize a new range and generate
    realDist = RealDist(rangeMin, rangeMax);
    return realDist(engine);
}
