#pragma once

#include <SDL.h>

class EventManager {
public:
    static void pollEvent(SDL_Window* window); // poll events and return true if application should quit
    static bool shouldQuit;
};
