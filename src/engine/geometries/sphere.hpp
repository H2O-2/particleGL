#pragma once

#include "geometry.hpp"

// Sphere particle. It is in fact a circle (2D) with initial position (0,0) and radius 1

extern const int DEFAULT_SEGMENT_NUM;
extern const float SPHERE_BASE_SCALE;

class Sphere : public Geometry {
public:
    Sphere();
    void init();
    int getIndexNum() const;
private:
    int segmentNum; // number of triangles to form the circle
    float radius;
};
