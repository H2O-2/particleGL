#pragma once

#include <random>

// Utility class for generating random numbers

typedef std::uniform_int_distribution<int> IntDist;
typedef std::uniform_real_distribution<float> RealDist;

class RandGen {
public:
    typedef std::mt19937 RandEngine;

    RandGen(uint32_t seed = 100000);
    uint32_t getSeed();
    void setSeed(uint32_t seed);
    IntDist::result_type randInt(const int& rangeMin, const int& rangeMax);
    RealDist::result_type randReal(const float& rangeMin, const float& rangeMax);
private:
    uint32_t seed;
    RandEngine engine;
    IntDist intDist;
    RealDist realDist;
};
