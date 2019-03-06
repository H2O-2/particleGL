#pragma once

#include "camera/camera.hpp"
#include "emitter/emitter.hpp"
#include "eventManager/eventManager.hpp"
#include "renderer/renderer.hpp"

class ParticleGL {
public:
    typedef std::unique_ptr<Emitter> EmitterPtr;
    ParticleGL(unsigned int windowWidth, unsigned int windowHeight, float framerate, glm::vec3 bgColor = glm::vec3(0.0f));
    ~ParticleGL();

    void addEmitter();
    void setParticleType(const int& emitterIndex, ParticleType particleType);

    void bufferData();
    void render();
    bool shouldEnd();
private:
    SDL_Window* window;
    std::unique_ptr<Renderer> renderer;
    std::vector<EmitterPtr> emitters;
    RenderData renderData; // A map with VAO as key and geometry index number as value
    float secondPerFrame; // Time of one frame in second
    void updateRenderData();
};
