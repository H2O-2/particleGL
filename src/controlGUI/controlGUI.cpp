#include "controlGUI.hpp"

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

bool ControlGUI::renderMenu(const std::string& name) {
    return ImGui::CollapsingHeader(name.c_str());
}

void ControlGUI::render3dFloatSlider(const std::string& name, float* v) {
    ImGui::Text(name.c_str());
    ImGui::DragFloat3(("##" + name).c_str(), v, 1.0f, 0.0f, 0.0f, "%.1f");
}

void ControlGUI::render3dUnsignedIntSlider(const std::string& name, float* v, const float scaleFactor) {
    int temp[3] = {static_cast<int>(v[0] / scaleFactor), static_cast<int>(v[1] / scaleFactor), static_cast<int>(v[2] / scaleFactor)};
    ImGui::Text(name.c_str());
    ImGui::DragInt3(("##" + name).c_str(), temp);
    for (int i = 0; i < 3; ++i) {
        v[i] = temp[i] > 0 ? temp[i] * scaleFactor : 0;
    }
}

void ControlGUI::renderCheckbox(const char* name, bool* v) {
    ImGui::Checkbox(name, v);
}

void ControlGUI::renderColorEdit3(const std::string& name, float* v) {
    ImGui::Text(name.c_str());
    ImGui::ColorEdit3(("##" + name).c_str(), v);
}

void ControlGUI::renderIntSlider(const std::string& name, int* v, const int min, const int max) {
    ImGui::Text(name.c_str());
    ImGui::SliderInt(("##" + name).c_str(), v, min, max);
}

void ControlGUI::renderIntSlider(const std::string& name, int* v, const int min, const int max, const int scaleFactor) {
    int temp = *v / scaleFactor;
    ImGui::Text(name.c_str());
    ImGui::SliderInt(("##" + name).c_str(), &temp, min, max);
    *v = temp * scaleFactor;
}

void ControlGUI::renderIntSlider(const std::string& name, float* v, const int min, const int max, const float scaleFactor) {
    int temp = static_cast<int>(*v / scaleFactor);
    ImGui::Text(name.c_str());
    ImGui::SliderInt(("##" + name).c_str(), &temp, min, max);
    *v = temp * scaleFactor;
}

void ControlGUI::renderText(const char* name) {
    ImGui::Text(name);
}

void ControlGUI::renderTextInput(const std::string& name, char* buffer) {
    ImGui::Text(name.c_str());
    ImGui::InputText(("##" + name).c_str(), buffer, 256);
}

void ControlGUI::renderUnsignedIntDragger(const std::string& name, uint32_t* v) {
    int temp = static_cast<int>(*v);
    ImGui::Text(name.c_str());
    ImGui::DragInt(("##" + name).c_str(), &temp);
    *v = temp;
}

void ControlGUI::renderUnsignedIntDragger(const std::string& name, float* v, const int len, const float scaleFactor) {
    int temp = static_cast<int>(v[0] / scaleFactor);
    ImGui::Text(name.c_str());
    ImGui::DragInt(("##" + name).c_str(), &temp);
    v[0] = temp > 0 ? temp * scaleFactor : 0;

    for (int i = 1; i < len; ++i) {
        v[i] = v[0];
    }
}

void ControlGUI::renderFloatDragger(const std::string& name, float* v, const int len, const float dragSpeed, const float scaleFactor) {
    float temp = v[0] / scaleFactor;
    ImGui::Text(name.c_str());
    ImGui::DragFloat(("##" + name).c_str(), &temp, dragSpeed, 0.0f, 0.0f, "%.1f");
    v[0] = temp * scaleFactor;

    for (int i = 1; i < len; ++i) {
        v[i] = v[0];
    }
}

void ControlGUI::renderFloatSlider(const std::string& name, float* v, const float min, const float max, const float scaleFactor, const char* format) {
    float temp = *v / scaleFactor;
    ImGui::Text(name.c_str());
    ImGui::SliderFloat(("##" + name).c_str(), &temp, min, max, format);
    *v = temp * scaleFactor;
}

void ControlGUI::renderRadioBtnSelection(const char* name, int* btnNum, const std::vector<const char *>& labels) {
    ImGui::Text(name);

    for (size_t i = 0; i < labels.size() - 1; ++i) {
        ImGui::RadioButton(labels[i], btnNum, i);
        ImGui::SameLine();
    }

    ImGui::RadioButton(labels[labels.size() - 1], btnNum, labels.size() - 1);
}

