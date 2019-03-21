#pragma once

#if __linux__
    #include <SDL2/SDL.h>
#else
    #include <SDL.h>
#endif
#include <memory>
#include <vector>

#include <glm/gtc/matrix_transform.hpp>

#include "engine/camera/camera.hpp"
#include "consoleMsg/consoleMsg.hpp"
#include "../emitter/emitter.hpp"
#include "../resource/shaderParser.hpp"

extern const int DEFAULT_MSAA;
extern const float PLANE_SCALE;
extern const float DEFAULT_NEAR_PLANE;
extern const float DEFAULT_FAR_PLANE;

typedef std::map<uint32_t, int> RenderData;

class Renderer {
public:
    Renderer();
    Renderer(const uint32_t& windowWidth, const uint32_t& windowHeight, const float& framerate, const glm::vec3& bgColor, const int& msaaSample = DEFAULT_MSAA);

    SDL_Window* initWindow(); // Initialize window and return the pointer of it
    void initTimer(); // Initialize curTime
    void initShader(const Camera& camera); // Initialize shaders

    // Buffer particles attributes using instanced array
    void initParticleBuffer(const uint32_t& VAO); // Allocate buffer for particles
    // void bufferParticles(glm::mat4 modelMats[], glm::vec3 colors[] = NULL);

    void updateParticleBuffer(const uint32_t& VAO, const std::vector<float>& offsets); // Update particle info in the buffer

    bool isHidpi() const; // Returns true if the current display is HiDPI

    SDL_GLContext getGLContext() const;

    void setMSAASample(const int& sample); // Set sample level for MSAA

    void clean();

    void clearScreen();

    // Render particles
    template<typename Function>
    void renderEngine(const std::vector<std::shared_ptr<Emitter>>& emitters, const Camera& camera, Function update) {
        // Referenced from https://gafferongames.com/post/fix_your_timestep/
        float accumulator = 0.0f;
        float newTime = SDL_GetTicks() * 0.001f;
        float deltaTime = newTime - curTime;
        curTime = newTime;

        accumulator += deltaTime;

        while (accumulator >= secondPerFrame) {
            update(1.0f); // update with one full frame
            accumulator -= secondPerFrame;
        }

        // Calculate the portion of the partial frame left in the accumulator and update
        const float interpolation = accumulator / secondPerFrame;
        update(interpolation);

        shader.use();
        shader.setMat4("view", camera.getViewMatrix());
        shader.setMat4("projection", glm::perspective(glm::radians(camera.getZoom()), (float)windowWidth / (float)windowHeight, nearVanish, farVanish));

        // Render particles
        for (auto const& emitter : emitters) {
            // Base scale for different geometries
            glm::mat4 baseScale;
            // TODO: Very ugly way to integrate size of particle, use instanced model matrix instead
            baseScale = glm::scale(baseScale, glm::vec3(emitter->getBaseScale() * emitter->getParticleSize()));
            shader.setMat4("baseScale", baseScale);

            // Particle transformation
            glm::mat4 model;
            shader.setMat4("model", model);
            glBindVertexArray(emitter->getVAO());
            if (emitter->useEBO()) {
                glDrawElementsInstanced(emitter->getDrawMode(), emitter->getIndexNum(), GL_UNSIGNED_INT, 0, emitter->getOffsets().size() / 3);
            } else {
                glDrawArraysInstanced(emitter->getDrawMode(), 0, emitter->getIndexNum(), emitter->getOffsets().size() / 3);
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

    int msaaSample; // Level of MSAA
    float nearVanish; // Far distance from the camera when particle vanishes. The actual value is a tenth of this value
    float farVanish; // Near distance from the camera when particle vanishes. The actual value is a tenth of this value

    SDL_DisplayMode display;

    uint32_t instancedVBO;

    /***** TODO *****/
    void updateMSAA();
    /***** TODO *****/

    /***** DEBUG *****/
    ShaderParser shader;
    /***** DEBUG *****/
};
