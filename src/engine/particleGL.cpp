#include "particleGL.hpp"

ParticleGL::ParticleGL(unsigned int windowWidth, unsigned int windowHeight, float framerate, glm::vec3 bgColor) {
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
