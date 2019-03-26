#include <glad/glad.h>
#include "renderer.hpp"
#include "../../util/makeUnique.hpp"

const int DEFAULT_MSAA = 8;
const float PLANE_SCALE = 0.1f;
const float DEFAULT_NEAR_PLANE = 1.0f;
const float DEFAULT_FAR_PLANE = 20000.0f;

const int Renderer::OFFSET_POSN = 1;
const int Renderer::COLOR_POSN = 2;
const int Renderer::MODEL_MAT_POSN = 3;

Renderer::Renderer() {}

Renderer::Renderer(const uint32_t windowWidth, const uint32_t windowHeight, const float secondPerFrame,
                   const glm::vec3& bgColor, const int msaaSample) :
    bgColor(bgColor), currentRenderMode(DEFAULT_RENDER), msaaSample(msaaSample), secondPerFrame(secondPerFrame),
    windowWidth(windowWidth), windowHeight(windowHeight), nearVanish(DEFAULT_NEAR_PLANE * PLANE_SCALE),
    farVanish(DEFAULT_FAR_PLANE * PLANE_SCALE) {}

SDL_Window* Renderer::initWindow() {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        ConsoleMsg::errorMsg("FAILED TO INIT SDL!");
        exit(-1);
    }
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
#ifdef __APPLE__
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG);
#endif

    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, msaaSample);

    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);

    // Get current display info
    if (SDL_GetCurrentDisplayMode(0, &display)) {
        ConsoleMsg::warningMsg("Could not get display info");
    }

    this->window = SDL_CreateWindow("ParticleGL", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, windowWidth, windowHeight,
                                            SDL_WINDOW_OPENGL|SDL_WINDOW_ALLOW_HIGHDPI|SDL_WINDOW_RESIZABLE);
    if (window == NULL) {
        ConsoleMsg::errorMsg("FAILED TO CREATE WINDOW!");
        SDL_Quit();
        exit(-1);
    }

    SDL_GL_SetSwapInterval(1); // Enable vsync
    this->glContext = SDL_GL_CreateContext(window);
    SDL_GL_MakeCurrent(window, glContext);

    if (!gladLoadGL()) {
        ConsoleMsg::errorMsg("FAIL TO INITIALIZE GLAD!");
        SDL_Quit();
        exit(-1);
    }

    int framebufferWidth, framebufferHeight;
    SDL_GL_GetDrawableSize(window, &framebufferWidth, &framebufferHeight);
    glViewport(0, 0, framebufferWidth, framebufferHeight);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_MULTISAMPLE);

    return window;
}

void Renderer::initTimer() {
    curTime = SDL_GetTicks() * 0.001f;
}

void Renderer::initShader(const Camera& camera) {
    shaders.emplace(RenderMode::U_MODEL_U_COLOR, ShaderParser("shaders/geometry/geoUniformModelUniformColor.vert", "shaders/geometry/geoUniformColor.frag"));
    shaders.emplace(RenderMode::U_MODEL_V_COLOR, ShaderParser("shaders/geometry/geoUniformModelVaringColor.vert", "shaders/geometry/geoVaringColor.frag"));
    shaders.emplace(RenderMode::V_MODEL_U_COLOR, ShaderParser("shaders/geometry/geoVaringModelUniformColor.vert", "shaders/geometry/geoUniformColor.frag"));
    shaders.emplace(RenderMode::V_MODEL_V_COLOR, ShaderParser("shaders/geometry/geoVaringModelVaringColor.vert", "shaders/geometry/geoVaringColor.frag"));

    // Initialize all shaders and use the default one
    for (auto& shader : shaders) {
        shader.second.init();
    }
}

void Renderer::initParticleBuffer() {
    // Create buffer for instanced offset
    glGenBuffers(1, &instancedOffsetVBO);
    glBindBuffer(GL_ARRAY_BUFFER, instancedOffsetVBO);
    glBufferData(GL_ARRAY_BUFFER, MAX_PARTICLE_NUM * 3 * sizeof(float), NULL, GL_STREAM_DRAW);
    glEnableVertexAttribArray(OFFSET_POSN);
    glVertexAttribPointer(OFFSET_POSN, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glVertexAttribDivisor(OFFSET_POSN, 1);

    // Create buffer for instanced model matrices
    glGenBuffers(1, &instancedModelMatVBO);
    glBindBuffer(GL_ARRAY_BUFFER, instancedModelMatVBO);
    glBufferData(GL_ARRAY_BUFFER, MAX_PARTICLE_NUM * 16 * sizeof(float), NULL, GL_STREAM_DRAW);
    glVertexAttribPointer(MODEL_MAT_POSN, 4, GL_FLOAT, GL_FALSE, 16 * sizeof(float), (void*)0);
    glVertexAttribPointer(MODEL_MAT_POSN + 1, 4, GL_FLOAT, GL_FALSE, 16 * sizeof(float), (void*)(4 * sizeof(float)));
    glVertexAttribPointer(MODEL_MAT_POSN + 2, 4, GL_FLOAT, GL_FALSE, 16 * sizeof(float), (void*)(8 * sizeof(float)));
    glVertexAttribPointer(MODEL_MAT_POSN + 3, 4, GL_FLOAT, GL_FALSE, 16 * sizeof(float), (void*)(12 * sizeof(float)));
    glEnableVertexAttribArray(MODEL_MAT_POSN);
    glEnableVertexAttribArray(MODEL_MAT_POSN + 1);
    glEnableVertexAttribArray(MODEL_MAT_POSN + 2);
    glEnableVertexAttribArray(MODEL_MAT_POSN + 3);
    glVertexAttribDivisor(MODEL_MAT_POSN, 1);
    glVertexAttribDivisor(MODEL_MAT_POSN + 1, 1);
    glVertexAttribDivisor(MODEL_MAT_POSN + 2, 1);
    glVertexAttribDivisor(MODEL_MAT_POSN + 3, 1);

    // Create buffer for instanced color
    glGenBuffers(1, &instancedColorVBO);
    glBindBuffer(GL_ARRAY_BUFFER, instancedColorVBO);
    glBufferData(GL_ARRAY_BUFFER, MAX_PARTICLE_NUM * 3 * sizeof(float), NULL, GL_STREAM_DRAW);
    glEnableVertexAttribArray(COLOR_POSN);
    glVertexAttribPointer(COLOR_POSN, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glVertexAttribDivisor(COLOR_POSN, 1);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Renderer::updateParticleBuffer(const uint32_t VAO, const std::vector<float>& offsets) {
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, instancedOffsetVBO);
    glBufferSubData(GL_ARRAY_BUFFER, 0, offsets.size() * sizeof(float), offsets.data());
    glEnableVertexAttribArray(OFFSET_POSN);
    glVertexAttribPointer(OFFSET_POSN, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glVertexAttribDivisor(OFFSET_POSN, 1);
}

void Renderer::updateParticleBuffer(const uint32_t VAO, const std::vector<float>& offsets, const std::vector<float>& colors) {
    updateParticleBuffer(VAO, offsets);

    glBindBuffer(GL_ARRAY_BUFFER, instancedColorVBO);
    glBufferSubData(GL_ARRAY_BUFFER, 0, colors.size() * sizeof(float), colors.data());
    glEnableVertexAttribArray(COLOR_POSN);
    glVertexAttribPointer(COLOR_POSN, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glVertexAttribDivisor(COLOR_POSN, 1);
}

void Renderer::updateParticleBufferWithMatrices(const uint32_t VAO, const std::vector<float>& modelMatrices) {
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, instancedModelMatVBO);
    glBufferSubData(GL_ARRAY_BUFFER, 0, modelMatrices.size() * sizeof(float), modelMatrices.data());
    glEnableVertexAttribArray(MODEL_MAT_POSN);
    glEnableVertexAttribArray(MODEL_MAT_POSN + 1);
    glEnableVertexAttribArray(MODEL_MAT_POSN + 2);
    glEnableVertexAttribArray(MODEL_MAT_POSN + 3);
    glVertexAttribPointer(MODEL_MAT_POSN, 4, GL_FLOAT, GL_FALSE, 16 * sizeof(float), (void*)0);
    glVertexAttribPointer(MODEL_MAT_POSN + 1, 4, GL_FLOAT, GL_FALSE, 16 * sizeof(float), (void*)(4 * sizeof(float)));
    glVertexAttribPointer(MODEL_MAT_POSN + 2, 4, GL_FLOAT, GL_FALSE, 16 * sizeof(float), (void*)(8 * sizeof(float)));
    glVertexAttribPointer(MODEL_MAT_POSN + 3, 4, GL_FLOAT, GL_FALSE, 16 * sizeof(float), (void*)(12 * sizeof(float)));
    glVertexAttribDivisor(MODEL_MAT_POSN, 1);
    glVertexAttribDivisor(MODEL_MAT_POSN + 1, 1);
    glVertexAttribDivisor(MODEL_MAT_POSN + 2, 1);
    glVertexAttribDivisor(MODEL_MAT_POSN + 3, 1);
}

void Renderer::updateParticleBufferWithMatrices(const uint32_t VAO, const std::vector<float>& modelMatrices, const std::vector<float>& colors) {
    updateParticleBufferWithMatrices(VAO, modelMatrices);

    glBindBuffer(GL_ARRAY_BUFFER, instancedColorVBO);
    glBufferSubData(GL_ARRAY_BUFFER, 0, colors.size() * sizeof(float), colors.data());
    glEnableVertexAttribArray(COLOR_POSN);
    glVertexAttribPointer(COLOR_POSN, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glVertexAttribDivisor(COLOR_POSN, 1);
}

SDL_GLContext Renderer::getGLContext() const {
    return glContext;
}

RenderMode Renderer::getCurrentRenderMode() const {
    return currentRenderMode;
}

void Renderer::setMSAASample(const int& sample) {
    if (msaaSample != sample) {
        msaaSample = sample;
        updateMSAA();
    }
}

bool Renderer::isHidpi() const {
    return display.w > 2048;
}

void Renderer::clean() {
    SDL_GL_DeleteContext(glContext);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void Renderer::clearScreen() {
    glClearColor(bgColor.x, bgColor.y, bgColor.z, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

/***** Private *****/

void Renderer::updateCurrentRenderMode(const std::vector<std::shared_ptr<Emitter>>& emitters) {
    bool colorVaring = false;
    bool modelVaring = false;

    for (auto const& emitter : emitters) {
        RenderMode curMode = emitter->getRenderMode();
        switch (curMode) {
            case RenderMode::U_MODEL_V_COLOR:
                colorVaring = true;
                break;
            case RenderMode::V_MODEL_U_COLOR:
                modelVaring = true;
                break;
            case RenderMode::V_MODEL_V_COLOR:
                colorVaring = true;
                modelVaring = true;
                break;
            default:
                break;
        }
    }

    if (colorVaring && modelVaring) {
        currentRenderMode = RenderMode::V_MODEL_V_COLOR;
    } else if (colorVaring) {
        currentRenderMode = RenderMode::U_MODEL_V_COLOR;
    } else if (modelVaring) {
        currentRenderMode = RenderMode::V_MODEL_U_COLOR;
    } else {
        currentRenderMode = RenderMode::U_MODEL_U_COLOR;
    }
}

void Renderer::updateMSAA() {}
