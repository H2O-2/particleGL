#include "controlGUI.hpp"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_sdl.h"
#include "imgui/imgui_impl_opengl3.h"

const char* GLSL_VERSION = "#version 330";

void ControlGUI::init(SDL_Window* window, SDL_GLContext glContext, bool isHiDpi) {
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

// Imgui doesn't seem to be working normally on HiDPI screen under Linux, so manully set font size
#if __linux__
    if (isHiDpi) {
        ImGuiIO& io = ImGui::GetIO();
        io.FontGlobalScale = 2.5;
    }
#endif

    ImGui::StyleColorsDark();

    ImGui_ImplSDL2_InitForOpenGL(window, glContext);
    ImGui_ImplOpenGL3_Init(GLSL_VERSION);
}

void ControlGUI::destroy() {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();
}

void ControlGUI::preRender(SDL_Window* window) {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL2_NewFrame(window);
    ImGui::NewFrame();

    ImGui::Begin("Control Panel");
}

void ControlGUI::finalizeRender() {
    ImGui::End();

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

bool ControlGUI::renderMenu(std::string name) {
    return ImGui::CollapsingHeader(name.c_str());
}

void ControlGUI::render3dFloatSlider(std::string name, float* v) {
    ImGui::Text(name.c_str());
    ImGui::DragFloat3(("##" + name).c_str(), v);
}

void ControlGUI::renderColorEdit3(std::string name, float* v) {
    ImGui::Text(name.c_str());
    ImGui::ColorEdit3(("##" + name).c_str(), v);
}

void ControlGUI::renderIntSlider(std::string name, int* v, const int min, const int max) {
    ImGui::Text(name.c_str());
    ImGui::SliderInt(("##" + name).c_str(), v, min, max);
}

void ControlGUI::renderIntSlider(std::string name, float* v, const int min, const int max, const float scaleFactor) {
    int temp = static_cast<int>(*v / scaleFactor);
    ImGui::Text(name.c_str());
    ImGui::SliderInt(("##" + name).c_str(), &temp, min, max);
    *v = temp * scaleFactor;
}

void ControlGUI::renderFloatSlider(std::string name, float* v, const float min, const float max, const float scaleFactor) {
    float temp = *v / scaleFactor;
    ImGui::Text(name.c_str());
    ImGui::SliderFloat(("##" + name).c_str(), &temp, min, max, "%.1f");
    *v = temp * scaleFactor;
}

void ControlGUI::renderPullDownMenu(std::string name, const std::vector<const char *>& items, int* currentItem) {
    ImGui::Text(name.c_str());
    ImGui::Combo(("##" + name).c_str(), currentItem, items.data(), items.size());
}

void ControlGUI::renderRadioBtnSelection(const char* name, int* btnNum, const std::vector<const char *>& labels) {
    ImGui::Text(name);

    for (size_t i = 0; i < labels.size() - 1; ++i) {
        ImGui::RadioButton(labels[i], btnNum, i);
        ImGui::SameLine();
    }

    ImGui::RadioButton(labels[labels.size() - 1], btnNum, labels.size() - 1);
}

