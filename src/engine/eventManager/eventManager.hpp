#pragma once

#if __linux__
    #include <SDL2/SDL.h>
#else
    #include <SDL.h>
#endif

#include "engine/camera/camera.hpp"

class EventManager {
public:
    static const int ZOOM_SPEED;

    static void pollEvent(SDL_Window* window, Camera& camera); // poll events and return true if application should quit
    static bool shouldQuit;
    static bool enableMouseView;
};
