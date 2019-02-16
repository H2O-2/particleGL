#pragma once

#include "../engine/particleGL.hpp"

#include <SDL.h>

class ControlGUI {
public:
    const char* glslVersion;

    ControlGUI(SDL_Window* window, SDL_GLContext glContext, ParticleGL& engine);
    ~ControlGUI();
    void render(SDL_Window* window);
    void destroy();
private:
    bool showDemoWindow;
};
