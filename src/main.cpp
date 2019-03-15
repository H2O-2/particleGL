#include "engine/particleGL.hpp"

#if __linux__
const unsigned int WINDOW_WIDTH = 2560;
const unsigned int WINDOW_HEIGHT = 1440;
#else
const unsigned int WINDOW_WIDTH = 1280;
const unsigned int WINDOW_HEIGHT = 720;
#endif
const float FRAME_RATE = 30.0;

int main(int, char**) {
    ParticleGL engine(WINDOW_WIDTH, WINDOW_HEIGHT, FRAME_RATE);
    engine.initBuffer();
    while (!engine.shouldEnd()) {
        engine.render();
    }

    return 0;
}
