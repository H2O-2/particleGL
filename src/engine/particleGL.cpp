#include "particleGL.hpp"
#include "../controlGUI/controlGUI.hpp"

ParticleGL::ParticleGL(unsigned int windowWidth, unsigned int windowHeight, float framerate, glm::vec3 bgColor)
    : secondPerFrame(1.0f / framerate), renderer(Renderer(windowWidth, windowHeight, secondPerFrame, bgColor)) {
    // Initialize renderer & OpenGL context
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
    for (size_t i = 0; i < emitters.size(); ++i) {
        // Buffer instanced array
        renderer.initParticleBuffer();
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
        if (ControlGUI::renderMenu("Emitter (Master)")) {
            ControlGUI::renderIntSlider("Particles/sec", (int *)emitter->getParticlesPerSecPtr(), 0, 1000);
            ControlGUI::render3dFloatSlider("Position", emitter->getEmitterPosnPtr());
            ControlGUI::renderFloatSlider("Velocity", emitter->getInitialVelocityPtr(), 0.0f, 1000.0f, PARTICLE_VELOCITY_SCALE);
            ControlGUI::renderIntSlider("Velocity Random [%%]", emitter->getInitialVelocityRandomnessPtr(), 0, 100, RANDOMNESS_SCALE);
            ControlGUI::renderFloatSlider("Velocity Distribution", emitter->getInitialVelocityRandomnessDistributionPtr(), 0.0f, 1.0);
        }

        if (ControlGUI::renderMenu("Particle (Master)")) {
            ControlGUI::renderPullDownMenu("Particle Type", {"Sphere", "Square", "Triangle"}, (int *)&(emitter->newParticleType));
            ControlGUI::renderFloatSlider("Life [sec]", emitter->getParticleLifePtr(), 0.0f, 10.0f);
            ControlGUI::renderIntSlider("Life Random [%%]", emitter->getParticleLifeRandomnessPtr(), 0, 100, RANDOMNESS_SCALE);
            ControlGUI::renderFloatSlider("Size", emitter->getParticleSizePtr(), 0.0f, 100.0f, PARTICLE_SIZE_SCALE);
            ControlGUI::renderIntSlider("Size Random [%%]", emitter->getParticleSizeRandomnessPtr(), 0, 100, RANDOMNESS_SCALE);
            ControlGUI::renderColorEdit3("Color", emitter->getParticleColorPtr());
            ControlGUI::renderIntSlider("Color Random", emitter->getParticleColorRandomnessPtr(), 0, 100, RANDOMNESS_SCALE);
        }

        if (ControlGUI::renderMenu("Physics (Master)")) {

        }

        if (ControlGUI::renderMenu("Visibility")) {

        }

        if (ControlGUI::renderMenu("Rendering")) {

        }
    }
    ControlGUI::finalizeRender();

    // Render
    renderer.renderEngine(emitters, camera, [&](const float& interpolation) {
        for (auto const& emitter : emitters) {
            // Update emitter & particle data
            emitter->setParticleType(emitter->newParticleType);
            emitter->update(interpolation);
            switch (renderer.getCurrentRenderMode()) {
                case RenderMode::U_MODEL_U_COLOR:
                    renderer.updateParticleBuffer(emitter->getVAO(), emitter->getOffsets());
                    break;
                case RenderMode::U_MODEL_V_COLOR:
                    renderer.updateParticleBuffer(emitter->getVAO(), emitter->getOffsets(), emitter->getInstancedColors());
                    break;
                case RenderMode::V_MODEL_U_COLOR:
                    renderer.updateParticleBufferWithMatrices(emitter->getVAO(), emitter->getModelMatrices());
                    break;
                case RenderMode::V_MODEL_V_COLOR:
                    renderer.updateParticleBufferWithMatrices(emitter->getVAO(), emitter->getModelMatrices(), emitter->getInstancedColors());
                    break;
                default:
                    break;
            }
        }

    });
}

bool ParticleGL::shouldEnd() const {
    return EventManager::shouldQuit;
}
