#pragma once

#if __linux__
    #include <SDL2/SDL.h>
#else
    #include <SDL.h>
#endif
#include <memory>
#include <vector>

#include <glm/gtc/matrix_transform.hpp>

#include "renderBuffer.hpp"
#include "engine/camera/camera.hpp"
#include "consoleMsg/consoleMsg.hpp"
#include "../emitter/emitter.hpp"
#include "../resource/shaderParser.hpp"

// Type of blend between particles
enum class ParticleBlend {
    NORMAL, // Blend according to opacity, this is the default setting
    ADD,
    SCREEN,
    LIGHTEN
};

extern const int DEFAULT_MSAA;
extern const float PLANE_SCALE;
extern const float DEFAULT_NEAR_PLANE;
extern const float DEFAULT_FAR_PLANE;
extern const int DEFAULT_FEATHER;
extern const ParticleBlend INIT_BLEND_TYPE;
extern const float INIT_COLOR_BLEND;

class Renderer {
public:
    Renderer();
    Renderer(const uint32_t windowWidth, const uint32_t windowHeight, const float framerate, const glm::vec3& bgColor, const int msaaSample = DEFAULT_MSAA);
    ~Renderer();

    SDL_Window* initWindow(); // Initialize window and return the pointer of it
    void initShader(const Camera& camera); // Initialize shaders

    // Buffer particles attributes using instanced array
    void initParticleBuffer(const uint32_t VAO); // Allocate buffer for particles

    float* getColorBlendPtr();

    void setMSAASample(const int& sample); // Set sample level for MSAA

    void clean();

    void renderEngine(const std::vector<std::shared_ptr<Emitter>>& emitters, const Camera& camera, bool* paused); // Render particles
private:
    typedef std::unordered_map<bool, ShaderParser> ShaderPair;

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
    ParticleBlend blendType; // Type of blend applied to particles
    ParticleBlend prevBlendType;
    float colorBlend; // Indicates the blending level of custom color and texture, takes value from [0.0, 1.0]. This field is only applicable when particle type is SPRITE

    RenderBuffer particleFBO;
    RenderBuffer pingpongFBO[2];

    RenderMode currentRenderMode;
    int msaaSample; // Level of MSAA
    float secondPerFrame; // Time of one frame in second
    unsigned int windowWidth;
    unsigned int windowHeight;

    // Settings
    float nearVanish; // Far distance from the camera when particle vanishes. The actual value is a tenth of this value
    float farVanish; // Near distance from the camera when particle vanishes. The actual value is a tenth of this value
    int feather; // Level of feathering, this should be a per-emitter attribute but currrently only a global switch is support
    bool paused;

    std::unordered_map<RenderMode, ShaderPair> shaders; // Shaders corresponding to different render modes

    ShaderParser screenShader; // Shader to display framebuffer & possible post-render effects
    Square screenQuad; // Quad to hold framebuffer

    ShaderParser featherShader; // Shader to apply feather to particles

    void clearScreen();

    bool isHidpi() const; // Returns true if the current display is HiDPI

    void renderGUI(const std::vector<std::shared_ptr<Emitter>>& emitters, bool* pasued);

    void renderToScreenQuad();

    void updateBlendMode(); // Update blend mode of particles

    // Determines which render mode to use.
    // Returns the Varing shader if both Varing and Uniform exists to avoid shader switching overhead
    // (Need testing to make sure its the right thing to do)
    void updateCurrentRenderMode(const std::vector<std::shared_ptr<Emitter>>& emitters);

    void updateParticleStatus(const std::vector<std::shared_ptr<Emitter>>& emitters, const float interpolation, const bool paused) const; // Update emitter & particle data

    // Update particle info in the buffer. Different overrides corresponds to different render modes
    void updateParticleBuffer(const std::vector<float>& offsets); // U_Model_U_COLOR
    void updateParticleBuffer(const std::vector<float>& offsets, const std::vector<float>& colors); // U_MODEL_V_COLOR
    void updateParticleBufferWithMatrices(const std::vector<float>& modelMatrices); // V_MODEL_U_COLOR
    void updateParticleBufferWithMatrices(const std::vector<float>& modelMatrices, const std::vector<float>& colors); // V_MODEL_U_COLOR

    /***** TODO *****/
    void updateMSAA();
    /***** TODO *****/
};
