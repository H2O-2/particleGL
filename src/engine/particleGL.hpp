#pragma once

#include "emitter/emitter.hpp"
#include "eventManager/eventManager.hpp"
#include "renderer/renderer.hpp"

class ParticleGL {
public:
    ParticleGL(unsigned int windowWidth, unsigned int windowHeight, float framerate, glm::vec3 bgColor = glm::vec3(0.0f));
    ~ParticleGL();

    void addEmitter();
    void initBuffer(); // Allocate buffer for emitters created initially
    void render();
    bool shouldEnd() const;
private:
    float secondPerFrame; // Time of one frame in second

    Camera camera;
    SDL_Window* window;
    Renderer renderer;
    std::vector<std::shared_ptr<Emitter>> emitters;

    void initEmitterBuffer(const uint32_t VAO); // Allocate buffer for particles in given emitter
};
