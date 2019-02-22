#pragma once

#include "geometry.hpp"

class Triangle : public Geometry {
public:
    // Triangle();
    // Triangle(float angleOne, float angleTwo);
    Triangle(float angleOne = 60.0f, float angleTwo = 60.0f);
    void init();
private:
    float angleOne;
    float angleTwo;
};
