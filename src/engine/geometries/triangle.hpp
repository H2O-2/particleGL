#pragma once

#include "geometry.hpp"

extern const int TRIANGLE_INDEX;
extern const float ANGLE_ONE;
extern const float ANGLE_TWO;
extern const float TRIANGLE_BASE_SCALE;

class Triangle : public Geometry {
public:
    Triangle(float angleOne = ANGLE_ONE, float angleTwo = ANGLE_TWO);
    void init();
private:
    float angleOne;
    float angleTwo;
};
