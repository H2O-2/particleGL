#pragma once

#include "../engine/particleGL.hpp"

#include <GLFW/glfw3.h>

class ControlGUI {
public:
    const char* glslVersion;

    ControlGUI(GLFWwindow* window, ParticleGL& engine);
    ~ControlGUI();
    void render();
    void destroy();
private:
    bool showDemoWindow;
};
