#include "particleGL.hpp"

ParticleGL::ParticleGL(unsigned int windowWidth, unsigned int windowHeight, float framerate, glm::vec3 bgColor) {
    // Initialize OpenGL context
    renderer = make_unique<Renderer>(windowWidth, windowHeight, framerate, bgColor);
    this->window = renderer->initWindow();

    // Add first emitter
    addEmitter();
}

ParticleGL::~ParticleGL() {
    renderer->clean();
}

void ParticleGL::addEmitter() {
    emitters.emplace_back(make_unique<Emitter>());

    int curEmitterIndex = emitters.size() - 1;
    // Buffer instanced array
    renderer->bufferParticles(emitters[curEmitterIndex]->getVAO(), emitters[curEmitterIndex]->getOffsets());
    // Update render data
    renderData[emitters[curEmitterIndex]->getVAO()] = emitters[curEmitterIndex]->getIndexNum();
}

void ParticleGL::render() {
    // Process events
    EventManager::pollEvent(window);

    for (int i = 0; i < emitters.size(); ++i) {
        // Update emitter & particle data
        emitters[i]->update();
    }

    // Render
    renderer->renderEngine(renderData);
}

bool ParticleGL::shouldEnd() {
    return EventManager::shouldQuit;
}
