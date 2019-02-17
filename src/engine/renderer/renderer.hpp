#pragma once

#include <SDL.h>
#include <glm/glm.hpp>

class Renderer {
public:
    Renderer(unsigned int windowWidth, unsigned int windowHeight, float framerate, glm::vec3 bgColor = glm::vec3(0.0f));
    SDL_Window* initWindow(); // initialize window and return the pointer of it
    void renderGui(); // render GUI;
    void renderEngine();
    void clean();
private:
    unsigned int windowWidth;
    unsigned int windowHeight;
    float framerate;
    glm::vec3 bgColor;
    SDL_Window* window;
    SDL_GLContext glContext;
};
