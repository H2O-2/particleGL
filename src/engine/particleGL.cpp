#include "particleGL.hpp"

ParticleGL::ParticleGL(unsigned int windowWidth, unsigned int windowHeight, float framerate, glm::vec3 bgColor, uint16_t emitterNum) {
    // Initialize emitters
    for (uint16_t i = 0; i < emitterNum; ++i) {
        emitters.emplace_back(Emitter());
    }

    renderer = std::unique_ptr<Renderer>(new Renderer(windowWidth, windowHeight, framerate, bgColor));
    this->window = renderer->initWindow();
}

ParticleGL::~ParticleGL() {
    renderer->clean();
}

void ParticleGL::render() {
    EventManager::pollEvent(window);
    renderer->renderEngine();
}

bool ParticleGL::shouldEnd() {
    return EventManager::shouldQuit;
}
