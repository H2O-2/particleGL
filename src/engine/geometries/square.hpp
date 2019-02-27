#pragma once

#include "geometry.hpp"

extern float DEFAULT_ASPECT;
extern float SQUARE_INDEX;
extern float SQUARE_BASE_SCALE;

class Square : public Geometry {
public:
    Square(float aspectRatio = DEFAULT_ASPECT);
    void init();
    int getIndexNum();
private:
    float aspectRatio;
};
