#pragma once

#include <SDL.h>

#include "../resource/shaderParser.hpp"
#include "../emitter/emitter.hpp"

extern const int DEFAULT_MSAA;

class Renderer {
public:
    Renderer(unsigned int windowWidth, unsigned int windowHeight, float framerate, glm::vec3 bgColor);

    // buffer particles attributes using instanced array
    void bufferParticles(const uint32_t& VAO, glm::vec3 offsets[]);
    void bufferParticles(glm::mat4 modelMats[], glm::vec3 colors[] = NULL);

    void setMSAASample(const int& sample);

    SDL_Window* initWindow(); // initialize window and return the pointer of it
    void renderGui(); // render GUI;
    void renderEngine(uint32_t VAO, int indexNum); // render particles
    void clean();
private:
    glm::vec3 bgColor;
    float framerate;
    int msaaSample;

    unsigned int windowWidth;
    unsigned int windowHeight;

    SDL_Window* window;
    SDL_GLContext glContext;

    /***** TODO *****/
    void updateMSAA();
    /***** TODO *****/

    /***** DEBUG *****/
    std::unique_ptr<ShaderParser> shader;
    std::unique_ptr<Emitter> emitter;
    /***** DEBUG *****/
};
