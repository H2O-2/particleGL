#pragma once

#include <SDL.h>
#include <glm/glm.hpp>
#include <memory>

#include "../resource/shaderParser.hpp"

// REMOVE, RENDER EMITTER
#include "../emitter/emitter.hpp"

class Renderer {
public:
    Renderer(unsigned int windowWidth, unsigned int windowHeight, float framerate, glm::vec3 bgColor);

    // buffer particles attributes using instanced array
    void bufferParticles(uint32_t VAO, glm::vec3 offsets[]);
    void bufferParticles(glm::mat4 modelMats[], glm::vec3 colors[] = NULL);

    SDL_Window* initWindow(); // initialize window and return the pointer of it
    void renderGui(); // render GUI;
    void renderEngine(uint32_t VAO, int indexNum); // render particles
    void clean();
private:
    unsigned int windowWidth;
    unsigned int windowHeight;
    float framerate;
    glm::vec3 bgColor;
    SDL_Window* window;
    SDL_GLContext glContext;

    /***** DEBUG *****/
    std::unique_ptr<ShaderParser> shader;
    std::unique_ptr<Emitter> emitter;
    /***** DEBUG *****/
};
