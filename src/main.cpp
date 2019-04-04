#include "engine/particleGL.hpp"

const unsigned int WINDOW_WIDTH = 1280;
const unsigned int WINDOW_HEIGHT = 720;
const float FRAME_RATE = 30.0;

int main(int, char**) {
    ParticleGL engine(WINDOW_WIDTH, WINDOW_HEIGHT, FRAME_RATE);
    engine.initBuffer();
    while (!engine.shouldEnd()) {
        engine.render();
    }

    return 0;
}
