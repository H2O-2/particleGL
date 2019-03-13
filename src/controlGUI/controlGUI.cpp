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

void ControlGUI::render(SDL_Window* window) {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL2_NewFrame(window);
    ImGui::NewFrame();

    ImGui::Begin("Hello, world!");
    ImGui::Text("test");
    ImGui::End();

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}
