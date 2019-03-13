#include "particleGL.hpp"

ParticleGL::ParticleGL(unsigned int windowWidth, unsigned int windowHeight, float framerate, glm::vec3 bgColor) {
    // Initialize OpenGL context
    secondPerFrame = framerate / 60.0f;
    renderer = Renderer(windowWidth, windowHeight, secondPerFrame, bgColor);
    this->window = renderer.initWindow();

    // Add first emitter
    addEmitter();

    // Initialize timer
    renderer.initTimer();
}

ParticleGL::~ParticleGL() {
    renderer.clean();
}

void ParticleGL::addEmitter() {
    emitters.emplace_back(std::shared_ptr<Emitter>(new Emitter(secondPerFrame)));
}

void ParticleGL::setParticleType(const int& emitterIndex, ParticleType particleType) {
    emitters[emitterIndex]->setParticleType(particleType);
}

void ParticleGL::bufferData() {
    for (auto& emitter : emitters) {
        // Buffer instanced array
        renderer.bufferParticles(emitter->getVAO(), emitter->getOffsets());
    }
}

void ParticleGL::render() {
    // Process events
    EventManager::pollEvent(window);

    // Render
    renderer.renderEngine(emitters, [&](const float& interpolation) {
        for (int i = 0; i < emitters.size(); ++i) {
            // Update emitter & particle data
            emitters[i]->update(interpolation);
        }
    });
}

bool ParticleGL::shouldEnd() const {
    return EventManager::shouldQuit;
}

