#pragma once

#if __linux__
    #include <SDL2/SDL.h>
#else
    #include <SDL.h>
#endif

#include <vector>
#include <string>

#include "imgui/imgui.h"
#include "imgui/imgui_impl_sdl.h"
#include "imgui/imgui_impl_opengl3.h"

extern const char* GLSL_VERSION;

class ControlGUI {
public:
    static void init(SDL_Window* window, SDL_GLContext glContext, bool isHiDpi = false);
    static void preRender(SDL_Window* window);
    static void finalizeRender();
    static void destroy();

    static bool renderMenu(const std::string& name);

    static void render3dFloatSlider(const std::string& name, float* v);
    static void render3dUnsignedIntSlider(const std::string& name, float* v, const float scaleFactor = 1.0f);
    static void renderCheckbox(const char* name, bool* v);
    static void renderColorEdit3(const std::string& name, float* v);
    static void renderIntSlider(const std::string& name, int* v, const int min, const int max);
    static void renderIntSlider(const std::string& name, int* v, const int min, const int max, const int scaleFactor);
    static void renderIntSlider(const std::string& name, float* v, const int min, const int max, const float scaleFactor);
    static void renderText(const char* name);
    static void renderTextInput(const std::string& name, char* buffer);
    static void renderUnsignedIntDragger(const std::string& name, uint32_t* v);
    static void renderUnsignedIntDragger(const std::string& name, float* v, const int len, const float scaleFactor = 1.0f);
    static void renderFloatDragger(const std::string& name, float* v, const int len, const float dragSpeed = 1.0f, const float scaleFactor = 1.0f);
    static void renderFloatSlider(const std::string& name, float* v, const float min, const float max, const float scaleFactor = 1.0f, const char* format = "%.1f");
    static void renderRadioBtnSelection(const char* name, int* btnNum, const std::vector<const char *>& labels); // Render a group of radio buttons from which at most one item can be selected

    template <typename T>
        static void renderPullDownMenu(const std::string& name, const std::vector<const char *>& items, T* currentItem);
};

template <typename T>
void ControlGUI::renderPullDownMenu(const std::string& name, const std::vector<const char *>& items, T* currentItem) {
    ImGui::Text(name.c_str());
    // Hopefully we get enum class support in ImGui someday to avoid this ugly cast :(
    ImGui::Combo(("##" + name).c_str(), reinterpret_cast<int*>(currentItem), items.data(), items.size());
}
