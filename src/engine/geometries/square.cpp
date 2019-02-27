#include "square.hpp"

float DEFAULT_ASPECT = 1.0f;
float SQUARE_INDEX = 4;
float SQUARE_BASE_SCALE = 2.0f;

Square::Square(float aspectRatio) : Geometry(SQUARE_INDEX, SQUARE_BASE_SCALE), aspectRatio(aspectRatio) {
    posns.reserve(indexNum);
    indices.reserve(2);
}

void Square::init() {
    posns.emplace_back(0.5f, 0.5f, 0.0f);
    posns.emplace_back(0.5f, -0.5f, 0.0f);
    posns.emplace_back(-0.5f, -0.5f, 0.0f);
    posns.emplace_back(-0.5f, 0.5f, 0.0f);

    indices = {
        0, 1, 3,
        1, 2, 3
    };

    // Write to GPU
    bufferGeometry();
}

int Square::getIndexNum() {
    return indexNum;
}
