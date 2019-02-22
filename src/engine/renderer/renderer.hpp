#pragma once

#include <SDL.h>
#include <glm/glm.hpp>
#include <memory>

#include "engine/geometries/sphere.hpp"
#include "engine/geometries/triangle.hpp"
#include "engine/resource/shaderParser.hpp"

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

    /***** DEBUG *****/
    std::unique_ptr<Sphere> test;
    std::unique_ptr<Triangle> test2;
    std::unique_ptr<ShaderParser> shader;
    /***** DEBUG *****/
};
