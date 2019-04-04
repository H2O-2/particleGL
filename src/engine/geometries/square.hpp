#pragma once

#include "geometry.hpp"

extern float DEFAULT_ASPECT;
extern float SQUARE_INDEX;
extern float SQUARE_BASE_SCALE;

class Square : public Geometry {
public:
    Square(float aspectRatio = DEFAULT_ASPECT, bool isQuad = false);
    void init();
    void setAspectRatio(const float r);
private:
    float aspectRatio;
    bool isQuad;

    void initQuadVertices(); // Initialize quad for screen framebuffer
    void initSquareVertices(); // Initialize square particle
};
