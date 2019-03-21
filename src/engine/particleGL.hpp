#pragma once

#include "emitter/emitter.hpp"
#include "eventManager/eventManager.hpp"
#include "renderer/renderer.hpp"

class ParticleGL {
public:
    ParticleGL(unsigned int windowWidth, unsigned int windowHeight, float framerate, glm::vec3 bgColor = glm::vec3(0.0f));
    ~ParticleGL();

    void addEmitter();
    void initBuffer(); // Allocate buffer for particles in all emitters
    void render();
    bool shouldEnd() const;
private:
    Camera camera;
    SDL_Window* window;
    Renderer renderer;
    std::vector<std::shared_ptr<Emitter>> emitters;
    float secondPerFrame; // Time of one frame in second

    void setParticleType(const int& emitterIndex, ParticleType particleType);
};
