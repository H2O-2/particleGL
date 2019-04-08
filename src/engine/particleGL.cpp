#include "particleGL.hpp"

ParticleGL::ParticleGL(unsigned int windowWidth, unsigned int windowHeight, float framerate, glm::vec3 bgColor)
    : secondPerFrame(1.0f / framerate), paused(false),
      renderer(Renderer(windowWidth, windowHeight, secondPerFrame, bgColor)) {
    // Initialize renderer & OpenGL context
    this->window = renderer.initWindow();
    renderer.initShader(camera);

    initTimer();

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

    // Update particle status
    updateParticles();

    // Render
    renderer.renderEngine(emitters, camera, &paused);
}

bool ParticleGL::shouldEnd() const {
    return EventManager::shouldQuit;
}


/***** Private *****/

void ParticleGL::initEmitterBuffer(const uint32_t VAO) {
    renderer.initParticleBuffer(VAO);
}

void ParticleGL::initTimer() {
    curTime = SDL_GetTicks() * 0.001f;
}

void ParticleGL::updateParticles() {
    // Referenced from https://gafferongames.com/post/fix_your_timestep/
    float accumulator = 0.0f;
    float newTime = SDL_GetTicks() * 0.001f;
    float deltaTime = newTime - curTime;
    curTime = newTime;

    accumulator += deltaTime;

    while (accumulator >= secondPerFrame) {
        updateParticleStatus(1.0f); // update with one full frame
        accumulator -= secondPerFrame;
    }

    // Calculate the portion of the partial frame left in the accumulator and update
    const float interpolation = accumulator / secondPerFrame;
    updateParticleStatus(interpolation);
}

void ParticleGL::updateParticleStatus(const float interpolation) const {
    if (paused) return;

    for (auto const& emitter : emitters) {
        emitter->update(interpolation, Physics::getInstance());
    }
}

