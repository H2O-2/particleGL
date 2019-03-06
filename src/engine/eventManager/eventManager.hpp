#pragma once

#if __linux__
    #include <SDL2/SDL.h>
#else
    #include <SDL.h>
#endif

class EventManager {
public:
    static void pollEvent(SDL_Window* window); // poll events and return true if application should quit
    static bool shouldQuit;
};
