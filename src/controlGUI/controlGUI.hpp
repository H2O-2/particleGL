#pragma once

#if __linux__
    #include <SDL2/SDL.h>
#else
    #include <SDL.h>
#endif

#include <vector>
#include <string>

extern const char* GLSL_VERSION;

class ControlGUI {
public:
    static void init(SDL_Window* window, SDL_GLContext glContext, bool isHiDpi = false);
    static void preRender(SDL_Window* window);
    static void finalizeRender();
    static void destroy();

    static bool renderMenu(std::string name);

    static void renderColorEdit3(std::string name, float* v);
    static void renderIntSlider(std::string name, int* v, const int min, const int max);
    static void renderIntSlider(std::string name, float* v, const int min, const int max, const float scaleFactor);
    static void renderFloatSlider(std::string name, float* v, const float min, const float max, const float scaleFactor = 1.0f);
    static void renderRadioBtnSelection(const char* name, int* btnNum, const std::vector<const char *>& labels); // Render a group of radio buttons from which at most one item can be selected
};
