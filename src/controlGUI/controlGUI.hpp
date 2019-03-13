#pragma once

#if __linux__
    #include <SDL2/SDL.h>
#else
    #include <SDL.h>
#endif

extern const char* GLSL_VERSION;

class ControlGUI {
public:
    static void init(SDL_Window* window, SDL_GLContext glContext, bool isHiDpi = false);
    static void render(SDL_Window* window);
    static void destroy();
private:
    bool showDemoWindow;
};
