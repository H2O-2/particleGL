#include "particleGL.hpp"
#include "../controlGUI/controlGUI.hpp"

ParticleGL::ParticleGL(unsigned int windowWidth, unsigned int windowHeight, float framerate, glm::vec3 bgColor) {
    // Initialize renderer & OpenGL context
    secondPerFrame = 1.0f / framerate;
    renderer = Renderer(windowWidth, windowHeight, secondPerFrame, bgColor);
    this->window = renderer.initWindow();
    renderer.initTimer();
    renderer.initShader(camera);

    // Add first emitter
    addEmitter();

    // Initialize GUI
    ControlGUI::init(window, renderer.getGLContext(), renderer.isHidpi());
}

ParticleGL::~ParticleGL() {
    ControlGUI::destroy();
    renderer.clean();
}

void ParticleGL::addEmitter() {
    emitters.emplace_back(std::shared_ptr<Emitter>(new Emitter(secondPerFrame)));
}

void ParticleGL::initBuffer() {
    for (auto& emitter : emitters) {
        // Buffer instanced array
        renderer.initParticleBuffer(emitter->getVAO());
    }
}

void ParticleGL::render() {
    // Process events
    EventManager::pollEvent(window, camera);

    // Clear screen
    renderer.clearScreen();

    // Render GUI
    ControlGUI::preRender(window);
    for (auto& emitter : emitters) {
        ControlGUI::renderRadioBtnSelection("Particle Type", (int *)&(emitter->newParticleType), {"Sphere", "Square", "Triangle"});
        ControlGUI::renderIntSlider("Particles/sec", (int *)emitter->getParticlesPerSecPtr(), 0, 1000);
        ControlGUI::renderFloatSlider("Velocity", emitter->getInitialVelocityPtr(), 0.0f, 1000.0f);
    }
    ControlGUI::finalizeRender();

    // Render
    renderer.renderEngine(emitters, camera, [&](const float& interpolation) {
        for (int i = 0; i < emitters.size(); ++i) {
            // Update emitter & particle data
            emitters[i]->setParticleType(emitters[i]->newParticleType);
            emitters[i]->update(interpolation);
            renderer.updateParticleBuffer(emitters[i]->getVAO(), emitters[i]->getOffsets());
        }
    });
}

bool ParticleGL::shouldEnd() const {
    return EventManager::shouldQuit;
}

void ParticleGL::setParticleType(const int& emitterIndex, ParticleType particleType) {
}

