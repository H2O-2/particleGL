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
    Renderer(const uint32_t windowWidth, const uint32_t windowHeight, const float framerate, const glm::vec3& bgColor, const int msaaSample = DEFAULT_MSAA);

    SDL_Window* initWindow(); // Initialize window and return the pointer of it
    void initTimer(); // Initialize curTime
    void initShader(const Camera& camera); // Initialize shaders

    // Buffer particles attributes using instanced array
    void initParticleBuffer(); // Allocate buffer for particles

    SDL_GLContext getGLContext() const;

    bool isHidpi() const; // Returns true if the current display is HiDPI

    void setMSAASample(const int& sample); // Set sample level for MSAA

    void clean();

    void clearScreen();

    void renderEngine(const std::vector<std::shared_ptr<Emitter>>& emitters, const Camera& camera); // Update and render particles
private:
    static const int OFFSET_POSN;
    static const int MODEL_MAT_POSN;
    static const int COLOR_POSN;

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
    void updateCurrentRenderMode(const std::vector<std::shared_ptr<Emitter>>& emitters);

    void updateParticleStatus(const std::vector<std::shared_ptr<Emitter>>& emitters, const float& interpolation) const; // Update emitter & particle data

    // Update particle info in the buffer. Different overrides corresponds to different render modes
    void updateParticleBuffer(const uint32_t VAO, const std::vector<float>& offsets); // U_Model_U_COLOR
    void updateParticleBuffer(const uint32_t VAO, const std::vector<float>& offsets, const std::vector<float>& colors); // U_MODEL_V_COLOR
    void updateParticleBufferWithMatrices(const uint32_t VAO, const std::vector<float>& modelMatrices); // V_MODEL_U_COLOR
    void updateParticleBufferWithMatrices(const uint32_t VAO, const std::vector<float>& modelMatrices, const std::vector<float>& colors); // V_MODEL_U_COLOR

    /***** TODO *****/
    void updateMSAA();
    /***** TODO *****/
};
