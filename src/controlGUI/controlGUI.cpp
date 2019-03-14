#include "controlGUI.hpp"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_sdl.h"
#include "imgui/imgui_impl_opengl3.h"

#include <stdio.h>

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

void ControlGUI::renderRadioBtnSelection(const char* name, int* btnNum, const std::vector<const char *>& labels) {
    ImGui::Text(name);

    for (int i = 0; i < labels.size() - 1; ++i) {
        ImGui::RadioButton(labels[i], btnNum, i);
        ImGui::SameLine();
    }

    ImGui::RadioButton(labels[labels.size() - 1], btnNum, labels.size() - 1);
}

void renderIntInput(const char* name, const int& value) {

}
