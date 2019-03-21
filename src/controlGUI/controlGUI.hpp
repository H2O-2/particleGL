#pragma once

#if __linux__
    #include <SDL2/SDL.h>
#else
    #include <SDL.h>
#endif

#include <vector>

extern const char* GLSL_VERSION;

class ControlGUI {
public:
    static void init(SDL_Window* window, SDL_GLContext glContext, bool isHiDpi = false);
    static void preRender(SDL_Window* window);
    static void finalizeRender();
    static void destroy();

    static void renderRadioBtnSelection(const char* name, int* btnNum, const std::vector<const char *>& labels); // Render a group of radio buttons from which at most one item can be selected
    static void renderIntSlider(const char* name, int* v, const int min, const int max);
    static void renderFloatSlider(const char* name, float* v, const float min, const float max);
private:
    bool showDemoWindow;
};
