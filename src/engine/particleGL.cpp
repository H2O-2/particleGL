#include "particleGL.hpp"

ParticleGL::ParticleGL(unsigned int windowWidth, unsigned int windowHeight, float framerate, glm::vec3 bgColor)
    : secondPerFrame(1.0f / framerate), renderer(Renderer(windowWidth, windowHeight, secondPerFrame, bgColor)) {
    // Initialize renderer & OpenGL context
    this->window = renderer.initWindow();
    renderer.initTimer();
    renderer.initShader(camera);

    // Add first emitter
    addEmitter();
}

ParticleGL::~ParticleGL() {
    renderer.clean();
}

void ParticleGL::addEmitter() {
    emitters.emplace_back(std::shared_ptr<Emitter>(new Emitter(secondPerFrame)));
}

void ParticleGL::initBuffer() {
    for (auto const& emitter : emitters) {
        initEmitterBuffer(emitter->getVAO());
    }
}

void ParticleGL::render() {
    // Process events
    EventManager::pollEvent(window, camera);

    // Render
    renderer.renderEngine(emitters, camera);
}

bool ParticleGL::shouldEnd() const {
    return EventManager::shouldQuit;
}


/***** Private *****/
void ParticleGL::initEmitterBuffer(const uint32_t VAO) {
    renderer.initParticleBuffer(VAO);
}

