#include "particleGL.hpp"
#include "../controlGUI/controlGUI.hpp"

ParticleGL::ParticleGL(unsigned int windowWidth, unsigned int windowHeight, float framerate, glm::vec3 bgColor) {
    // Initialize OpenGL context
    secondPerFrame = framerate / 60.0f;
    renderer = Renderer(windowWidth, windowHeight, secondPerFrame, bgColor);
    this->window = renderer.initWindow();

    ControlGUI::init(window, renderer.getGLContext(), renderer.isHidpi());

    // Add first emitter
    addEmitter();

    // Initialize timer
    renderer.initTimer();
}

ParticleGL::~ParticleGL() {
    ControlGUI::destroy();
    renderer.clean();
}

void ParticleGL::addEmitter() {
    emitters.emplace_back(std::shared_ptr<Emitter>(new Emitter(secondPerFrame)));
}

void ParticleGL::setParticleType(const int& emitterIndex, ParticleType particleType) {
    bufferData(emitterIndex);
    emitters[emitterIndex]->setParticleType(particleType);
}

void ParticleGL::bufferData() {
    for (auto& emitter : emitters) {
        // Buffer instanced array
        renderer.bufferParticles(emitter->getVAO(), emitter->getOffsets());
    }
}

void ParticleGL::bufferData(const int &emitterIndex) {
    renderer.bufferParticles(emitters[emitterIndex]->getVAO(), emitters[emitterIndex]->getOffsets());
}

void ParticleGL::render() {
    // Process events
    EventManager::pollEvent(window);

    // Clear screen
    renderer.clearScreen();

    // Render GUI
    ControlGUI::preRender(window);
    for (auto& emitter : emitters) {
        ControlGUI::renderRadioBtnSelection("Particle Type", (int *)&(emitter->newParticleType), {"Sphere", "Square", "Triangle"});
    }
    ControlGUI::finalizeRender();

    // Render
    renderer.renderEngine(emitters, [&](const float& interpolation) {
        for (int i = 0; i < emitters.size(); ++i) {
            // Update emitter & particle data
            setParticleType(i, emitters[i]->newParticleType);
            emitters[i]->update(interpolation);
        }
    });
}

bool ParticleGL::shouldEnd() const {
    return EventManager::shouldQuit;
}

