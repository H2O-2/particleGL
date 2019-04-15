#pragma once

#include <random>
#include <glm/glm.hpp>

// Utility class for generating random numbers

typedef std::uniform_int_distribution<int> IntDist;
typedef std::uniform_real_distribution<float> RealDist;

class RandGen {
public:
    typedef std::mt19937 RandEngine;

    RandGen(uint32_t seed = 100000);
    uint32_t getSeed();
    void setSeed(uint32_t seed);
    IntDist::result_type randIntClosed(const int rangeMin, const int rangeMax);
    RealDist::result_type randRealClosed(const float rangeMin, const float rangeMax);
    RealDist::result_type randRealOpenLeft(const float rangeMin, const float rangeMax);
    RealDist::result_type randRealOpenRight(const float rangeMin, const float rangeMax);
    glm::vec3 randVec3Closed(const float rangeMin, const float rangeMax);
    glm::vec3 randVec3Closed(const glm::vec3& rangeMin, const glm::vec3& rangeMax);
private:
    uint32_t seed;
    RandEngine engine;
    IntDist intDist;
    RealDist realDist;

    float closedBound(const float bound) const; // Returns the float to be used as the upper bound of uniform_real_distribution to form a closed interval with `bound` as upper bound
};
