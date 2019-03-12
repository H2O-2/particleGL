#include "sphere.hpp"

#define _USE_MATH_DEFINES
#include <math.h>

const int DEFAULT_SEGMENT_NUM = 360;
const float SPHERE_BASE_SCALE = 1.0f;

Sphere::Sphere() : Geometry(DEFAULT_SEGMENT_NUM + 2, SPHERE_BASE_SCALE, GL_TRIANGLE_FAN), segmentNum(DEFAULT_SEGMENT_NUM), radius(1) {
    posns.reserve(indexNum);
}

void Sphere::init() {
    float unitRadian = (float)(2 * M_PI / segmentNum);

    // Push the center of the circle
    posns.emplace_back(0.0f);

    for (int i = 0; i < segmentNum; ++i) {
        float x = radius * cosf(unitRadian * i);
        float y = radius * sinf(unitRadian * i);
        posns.emplace_back(x, y, 0.0f);
    }

    // Close the circle
    posns.emplace_back(radius * cosf(unitRadian), radius * sinf(unitRadian), 0.0f);

    // Write to GPU
    bufferGeometry();
}

int Sphere::getIndexNum() const {
    return indexNum;
}
