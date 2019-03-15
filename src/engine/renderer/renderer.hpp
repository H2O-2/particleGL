#pragma once

#if __linux__
    #include <SDL2/SDL.h>
#else
    #include <SDL.h>
#endif
#include <memory>
#include <vector>

#include "consoleMsg/consoleMsg.hpp"
#include "../emitter/emitter.hpp"
#include "../resource/shaderParser.hpp"

#include <iostream>

extern const int DEFAULT_MSAA;

typedef std::map<uint32_t, int> RenderData;

class Renderer {
public:
    Renderer();
    Renderer(const uint32_t& windowWidth, const uint32_t& windowHeight, const float& framerate, const glm::vec3& bgColor, const int& msaaSample = DEFAULT_MSAA);

    SDL_Window* initWindow(); // Initialize window and return the pointer of it
    void initTimer(); // Initialize curTime

    // Buffer particles attributes using instanced array
    void initParticleBuffer(const uint32_t& VAO, const std::vector<float>& offsets); // Allocate buffer for particles
    // void bufferParticles(glm::mat4 modelMats[], glm::vec3 colors[] = NULL);

    void updateParticleBuffer(const uint32_t& VAO, const std::vector<float>& offsets); // Update particle info in the buffer

    bool isHidpi() const; // Returns true if the current display is HiDPI

    SDL_GLContext getGLContext() const;

    void setMSAASample(const int& sample); // Set sample level for MSAA

    void clean();

    void clearScreen();

    // Render particles
    template<typename Function>
    void renderEngine(const std::vector<std::shared_ptr<Emitter>>& emitters, Function update) {
        // Referenced from https://gafferongames.com/post/fix_your_timestep/
        float newTime = SDL_GetTicks() * 0.001f;
        float deltaTime = newTime - curTime;
        curTime = newTime;

        accumulator += deltaTime;

        while (accumulator >= secondPerFrame) {
            update(1.0f); // update with one full frame
            accumulator -= secondPerFrame;
        }

        const float interpolation = accumulator / secondPerFrame;

        // Update particle info
        update(interpolation);

        // Render particles
        for (auto const& emitter : emitters) {
            glBindVertexArray(emitter->getVAO());
            if (emitter->useEBO()) {
                glDrawElementsInstanced(emitter->getDrawMode(), emitter->getIndexNum(), GL_UNSIGNED_INT, 0, 50);
            } else {
                glDrawArraysInstanced(emitter->getDrawMode(), 0, emitter->getIndexNum(), 50);
            }
            glBindVertexArray(0);
        }

        SDL_GL_SwapWindow(window);
    }
private:
    unsigned int windowWidth;
    unsigned int windowHeight;

    glm::vec3 bgColor;
    float secondPerFrame; // Time of one frame in second

    SDL_Window* window;
    SDL_GLContext glContext;

    float curTime;
    float accumulator;

    int msaaSample;

    SDL_DisplayMode display;

    uint32_t instancedVBO;

    /***** TODO *****/
    void updateMSAA();
    /***** TODO *****/

    /***** DEBUG *****/
    std::unique_ptr<ShaderParser> shader;
    /***** DEBUG *****/
};
