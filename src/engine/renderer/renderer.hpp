#pragma once

#include <SDL.h>

#include "../resource/shaderParser.hpp"
#include "../emitter/emitter.hpp"

extern const int DEFAULT_MSAA;

typedef std::map<uint32_t, int> RenderData;

class Renderer {
public:

    Renderer(unsigned int windowWidth, unsigned int windowHeight, float framerate, glm::vec3 bgColor);

    // Buffer particles attributes using instanced array
    void bufferParticles(const uint32_t& VAO, glm::vec3 offsets[]);
    void bufferParticles(glm::mat4 modelMats[], glm::vec3 colors[] = NULL);

    void setMSAASample(const int& sample);

    SDL_Window* initWindow(); // Initialize window and return the pointer of it
    void renderGui(); // Render GUI;
    void renderEngine(const RenderData& renderData); // Render particles
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
    /***** DEBUG *****/
};
