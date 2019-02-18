#include "sphere.hpp"

#define _USE_MATH_DEFINES
#include <math.h>

Sphere::Sphere() : Geometry(), segmentNum(360), radius(1) {
    posns.reserve(segmentNum + 2);
}

void Sphere::init() {
    float unitRadian = (float)(2 * M_PI / segmentNum);

    // Push the center of the circle
    posns.emplace_back(glm::vec3(0.0f));

    for (int i = 0; i < segmentNum; ++i) {
        float x = radius * cosf(unitRadian * i);
        float y = radius * sinf(unitRadian * i);
        posns.emplace_back(glm::vec3(x, y, 0.0f));
    }

    // Close the circle
    // posns.emplace_back(glm::vec3(radius * cosf(unitRadian), radius * sinf(unitRadian), 0.0f));

    // Write to GPU
    bufferGeometry();
}

int Sphere::getSegmentNum() {
    return segmentNum;
}
