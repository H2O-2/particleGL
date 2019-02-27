#include "particleGL.hpp"

ParticleGL::ParticleGL(unsigned int windowWidth, unsigned int windowHeight, float framerate, glm::vec3 bgColor, uint16_t emitterNum) {
    // Initialize OpenGL context
    renderer = make_unique<Renderer>(windowWidth, windowHeight, framerate, bgColor);
    this->window = renderer->initWindow();

    // Initialize emitters
    for (uint16_t i = 0; i < emitterNum; ++i) {
        emitters.emplace_back(make_unique<Emitter>());
        // Buffer data
        renderer->bufferParticles(emitters[i]->getVAO(), emitters[i]->getOffsets());
    }
}

ParticleGL::~ParticleGL() {
    renderer->clean();
}

void ParticleGL::render() {
    EventManager::pollEvent(window);

    for (int i = 0; i < emitters.size(); ++i) {
        // Render
        renderer->renderEngine(emitters[i]->getVAO(), emitters[i]->getIndexNum());
    }
}

bool ParticleGL::shouldEnd() {
    return EventManager::shouldQuit;
}
