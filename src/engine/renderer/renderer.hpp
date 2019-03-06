#pragma once

#if __linux__
    #include <SDL2/SDL.h>
#else
    #include <SDL.h>
#endif
#include <memory>

#include "../resource/shaderParser.hpp"

#include <iostream>

extern const int DEFAULT_MSAA;

typedef std::map<uint32_t, int> RenderData;

class Renderer {
public:

    Renderer(const uint32_t& windowWidth, const uint32_t& windowHeight, const float& framerate, const glm::vec3& bgColor, const int& msaaSample = DEFAULT_MSAA);

    // Buffer particles attributes using instanced array
    void bufferParticles(const uint32_t& VAO, glm::vec3 offsets[]);
    void bufferParticles(glm::mat4 modelMats[], glm::vec3 colors[] = NULL);

    void setMSAASample(const int& sample); // Set sample level for MSAA

    SDL_Window* initWindow(); // Initialize window and return the pointer of it
    void initTimer(); // Initialize curTime

    void renderGui(); // Render GUI;

    void clean();

    // Render particles
    template<typename Function>
    void renderEngine(const RenderData& renderData, Function update) {
        // Referenced from https://gafferongames.com/post/fix_your_timestep/
        float newTime = SDL_GetTicks() * 0.001f;
        float deltaTime = newTime - curTime;
        curTime = newTime;

        accumulator += deltaTime;

        while (accumulator >= secondPerFrame) {
            update(0.0f);
            accumulator -= secondPerFrame;
        }

        const float interpolation = accumulator / secondPerFrame;
        update(interpolation);

        // Render particles
        glClearColor(bgColor.x, bgColor.y, bgColor.z, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        for (auto const& data : renderData) {
            // std::cout << glGetError() << '\n';
            glBindVertexArray(data.first);
            glDrawElementsInstanced(GL_TRIANGLES, data.second, GL_UNSIGNED_INT, 0, 200);
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

    /***** TODO *****/
    void updateMSAA();
    /***** TODO *****/

    /***** DEBUG *****/
    std::unique_ptr<ShaderParser> shader;
    /***** DEBUG *****/
};
