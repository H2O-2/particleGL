#pragma once

#include "geometry.hpp"

class Triangle : public Geometry {
public:
    Triangle(float angleOne = 60.0f, float angleTwo = 60.0f);
    void init();
    int getIndexNum();
private:
    float angleOne;
    float angleTwo;
};
