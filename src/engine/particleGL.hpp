#pragma once

#include "camera/camera.hpp"
#include "emitter/emitter.hpp"
#include "eventManager/eventManager.hpp"
#include "renderer/renderer.hpp"

class ParticleGL {
public:
    std::vector<std::unique_ptr<Emitter>> emitters;
    ParticleGL(unsigned int windowWidth, unsigned int windowHeight, float framerate, glm::vec3 bgColor = glm::vec3(0.0f), uint16_t emitterNum = 1);
    ~ParticleGL();
    void render();
    bool shouldEnd();
private:
    SDL_Window* window;
    std::unique_ptr<Renderer> renderer;
};
