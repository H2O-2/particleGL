#pragma once

#include "camera/camera.hpp"
#include "emitter/emitter.hpp"
#include "eventManager/eventManager.hpp"
#include "renderer/renderer.hpp"

class ParticleGL {
public:
    ParticleGL(unsigned int windowWidth, unsigned int windowHeight, float framerate, glm::vec3 bgColor = glm::vec3(0.0f));
    ~ParticleGL();

    void addEmitter();
    void setParticleType(const int& emitterIndex, ParticleType particleType);

    void bufferData();
    void bufferData(const int& emitterIndex);
    void render();
    bool shouldEnd() const;
private:
    SDL_Window* window;
    Renderer renderer;
    std::vector<std::shared_ptr<Emitter>> emitters;
    float secondPerFrame; // Time of one frame in second
};
