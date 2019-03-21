#include "randGen.hpp"

#include <cmath>

RandGen::RandGen(uint32_t seed) : seed(seed), engine(RandEngine(seed)), boolDist(BoolDist(0.5)), intDist(IntDist(0, 101)), realDist(RealDist(0.0f, closedBound(0.5f))) {}

uint32_t RandGen::getSeed() {
    return seed;
}

void RandGen::setSeed(uint32_t seed) {
    this->seed = seed;
    engine.seed(seed);
}

IntDist::result_type RandGen::randIntClosed(const int& rangeMin, const int& rangeMax) {
    // If requested range is available, generate directly
    if (rangeMin == intDist.min() && rangeMax == intDist.max() - 1) {
        return intDist(engine);
    }

    // else re-initialize a new range and generate
    intDist = IntDist(rangeMin, rangeMax + 1);
    return intDist(engine);
}

RealDist::result_type RandGen::randRealClosed(const float& rangeMin, const float& rangeMax) {
    // Generate the upper bound to close the range of [rangeMin, rangeMax]
    float upperBound = closedBound(rangeMax);

    // If requested range is available, generate directly
    if (rangeMin == realDist.min() && upperBound == realDist.max()) {
        return realDist(engine);
    }

    // else re-initialize a new range and generate
    realDist = RealDist(rangeMin, upperBound);
    return realDist(engine);
}

RealDist::result_type RandGen::randRealOpenRight(const float& rangeMin, const float& rangeMax) {
    // If requested range is available, generate directly
    if (rangeMin == realDist.min() && rangeMax == realDist.max()) {
        return realDist(engine);
    }

    realDist = RealDist(rangeMin, rangeMax);
    return realDist(engine);
}

bool RandGen::randBool() {
    return boolDist(engine);
}


/***** Private *****/
float RandGen::closedBound(const float& bound) {
    // Reference: https://en.cppreference.com/w/cpp/numeric/random/uniform_real_distribution
    return std::nextafter(bound, std::numeric_limits<RealDist::result_type>::max());
}

