#include <glad/glad.h>
#include "renderer.hpp"
#include "../../util/makeUnique.hpp"

const int DEFAULT_MSAA = 8;
const float PLANE_SCALE = 0.1f;
const float DEFAULT_NEAR_PLANE = 1.0f;
const float DEFAULT_FAR_PLANE = 20000.0f;

Renderer::Renderer() {}

Renderer::Renderer(const uint32_t& windowWidth, const uint32_t& windowHeight, const float& secondPerFrame,
                   const glm::vec3& bgColor, const int& msaaSample) :
    windowWidth(windowWidth), windowHeight(windowHeight), bgColor(bgColor), secondPerFrame(secondPerFrame),
    msaaSample(msaaSample), nearVanish(DEFAULT_NEAR_PLANE * PLANE_SCALE), farVanish(DEFAULT_FAR_PLANE * PLANE_SCALE) {}

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
    shader = ShaderParser("shaders/geometry.vert", "shaders/geometry.frag");
    shader.init();
    shader.use();
    shader.setVec3("color", glm::vec3(1.0f));
}

void Renderer::initParticleBuffer(const uint32_t& VAO) {
    glGenBuffers(1, &instancedVBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, instancedVBO);
    glBufferData(GL_ARRAY_BUFFER, MAX_PARTICLE_NUM * sizeof(float), NULL, GL_STREAM_DRAW);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glVertexAttribDivisor(1, 1);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

// void Renderer::bufferParticles(glm::mat4 modelMats[], glm::vec3 colors[]) {}

void Renderer::updateParticleBuffer(const uint32_t& VAO, const std::vector<float>& offsets) {
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, instancedVBO);
    glBufferSubData(GL_ARRAY_BUFFER, 0, offsets.size() * sizeof(float), offsets.data());
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glVertexAttribDivisor(1, 1);
}

SDL_GLContext Renderer::getGLContext() const {
    return glContext;
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

void Renderer::updateMSAA() {}
