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

class Renderer {
public:
    Renderer();
    Renderer(const uint32_t& windowWidth, const uint32_t& windowHeight, const float& framerate, const glm::vec3& bgColor, const int& msaaSample = DEFAULT_MSAA);

    SDL_Window* initWindow(); // Initialize window and return the pointer of it
    void initTimer(); // Initialize curTime
    void initShader(const Camera& camera); // Initialize shaders

    // Buffer particles attributes using instanced array
    void initParticleBuffer(); // Allocate buffer for particles

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

        // Update render mode
        updateCurrentMode(emitters);

        ShaderParser& shader = shaders[currentRenderMode];
        shader.setMat4("view", camera.getViewMatrix());
        shader.setMat4("projection", glm::perspective(glm::radians(camera.getZoom()), (float)windowWidth / (float)windowHeight, nearVanish, farVanish));

        // Render particles
        for (auto const& emitter : emitters) {
            // Transforms for emitters
            glm::mat4 emitterModel;
            shader.setMat4("emitterModel", emitterModel);

            glm::mat4 particleModel;
            // Scaling and rotation for particles
            particleModel = glm::scale(particleModel, glm::vec3(emitter->getBaseScale() * emitter->getParticleSize()));
            shader.setMat4("particleModel", particleModel);

            shader.setVec3("color", emitter->getParticleColor());

            // Render particles
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
    // Rendering mode for particles.
    // Uniform Model will pass to shader only instanced translation, rotation and scale will be passed as uniform variables while Varing model will pass instanced model matrices to shader
    // Uniform Color will only pass color to fragment shdaer as uniform variable while Varing color will pass instanced colors to (vertex) shader
    enum class RenderMode {
        U_MODEL_U_COLOR, // Uniform Model Uniform Color
        U_MODEL_V_COLOR, // Uniform Model Varing Color
        V_MODEL_U_COLOR, // Varing Model Uniform Color
        V_MODEL_V_COLOR  // Uniform Model Varing Color
    };

    static const int OFFSET_POSN;
    static const int MODEL_MAT_POSN;
    static const int COLOR_POSN;
    static const RenderMode DEFAULT_RENDER;

    // Rendering objects
    SDL_GLContext glContext;
    SDL_DisplayMode display;
    SDL_Window* window;

    // VBOs
    uint32_t instancedOffsetVBO;
    uint32_t instancedModelMatVBO;
    uint32_t instancedColorVBO;

    // Rendering attributes
    glm::vec3 bgColor;
    RenderMode currentRenderMode;
    float curTime;
    int msaaSample; // Level of MSAA
    float secondPerFrame; // Time of one frame in second
    unsigned int windowWidth;
    unsigned int windowHeight;

    // Settings
    float nearVanish; // Far distance from the camera when particle vanishes. The actual value is a tenth of this value
    float farVanish; // Near distance from the camera when particle vanishes. The actual value is a tenth of this value

    std::unordered_map<RenderMode, ShaderParser> shaders; // Shaders corresponding to different render modes

    // Determines which render mode to use.
    // Returns the Varing shader if both Varing and Uniform exists to avoid shader switching overhead
    // (Need testing to make sure its the right thing to do)
    void updateCurrentMode(const std::vector<std::shared_ptr<Emitter>>& emitters);
    /***** TODO *****/
    void updateMSAA();
    /***** TODO *****/
};
