#include "randGen.hpp"

#include <cmath>

RandGen::RandGen(uint32_t seed) : seed(seed), engine(RandEngine(seed)), boolDist(), intDist(IntDist(0, 101)), realDist(RealDist(0.0f, closedBound(0.5f))) {}

uint32_t RandGen::getSeed() {
    return seed;
}

void RandGen::setSeed(uint32_t seed) {
    this->seed = seed;
    engine.seed(seed);
}

bool RandGen::randBool(const float p) {
    // If requested probability of true is available, generate directly
    if (p == boolDist.p()) {
        return boolDist(engine);
    }

    // else re-initialize a new possibility and generate
    boolDist = BoolDist(p);
    return boolDist(engine);
}

IntDist::result_type RandGen::randIntClosed(const int rangeMin, const int rangeMax) {
    // If requested range is available, generate directly
    if (rangeMin == intDist.min() && rangeMax == intDist.max() - 1) {
        return intDist(engine);
    }

    // else re-initialize a new range and generate
    intDist = IntDist(rangeMin, rangeMax + 1);
    return intDist(engine);
}

RealDist::result_type RandGen::randRealClosed(const float rangeMin, const float rangeMax) {
    // Generate the upper bound to close the range of [rangeMin, rangeMax]
    float upperBound = closedBound(rangeMax);

    return randRealOpenLeft(rangeMin, upperBound);
}

RealDist::result_type RandGen::randRealOpenLeft(const float rangeMin, const float rangeMax) {
    return -randRealOpenRight(-rangeMax, -rangeMin);
}

RealDist::result_type RandGen::randRealOpenRight(const float rangeMin, const float rangeMax) {
    // If requested range is available, generate directly
    if (rangeMin == realDist.min() && rangeMax == realDist.max()) {
        return realDist(engine);
    }

    // else re-initialize a new range and generate
    realDist = RealDist(rangeMin, rangeMax);
    return realDist(engine);
}

glm::vec3 RandGen::randVec3(const float rangeMin, const float rangeMax) {
    if (rangeMin != realDist.min() || rangeMax != realDist.max()) {
        realDist = RealDist(rangeMin, rangeMax);
    }

    return glm::vec3(realDist(engine), realDist(engine), realDist(engine));
}

/***** Private *****/
float RandGen::closedBound(const float bound) const {
    // Reference: https://en.cppreference.com/w/cpp/numeric/random/uniform_real_distribution
    return std::nextafter(bound, std::numeric_limits<RealDist::result_type>::max());
}

