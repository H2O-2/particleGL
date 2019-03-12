#include "triangle.hpp"

const int TRIANGLE_INDEX = 3;
const float ANGLE_ONE = 60.0f;
const float ANGLE_TWO = 60.0f;
const float TRIANGLE_BASE_SCALE = 2.0f;

Triangle::Triangle(float angleOne, float angleTwo) : Geometry(TRIANGLE_INDEX, TRIANGLE_BASE_SCALE), angleOne(angleOne), angleTwo(angleTwo) {
    posns.reserve(indexNum);
}

void Triangle::init() {
    posns.emplace_back(-0.5f, -0.5f, 0.0f);
    posns.emplace_back(0.5f, -0.5f, 0.0f);
    posns.emplace_back(0.0f,  0.5f, 0.0f);

    // Write to GPU
    bufferGeometry();
}

int Triangle::getIndexNum() const {
    return indexNum;
}
