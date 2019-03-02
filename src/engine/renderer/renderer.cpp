#include <glad/glad.h>
#include "renderer.hpp"
#include "consoleMsg/consoleMsg.hpp"
#include "../../util/makeUnique.hpp"

#include <glm/gtc/matrix_transform.hpp>

const int DEFAULT_MSAA = 8;

Renderer::Renderer(const uint32_t& windowWidth, const uint32_t& windowHeight, const float& secondPerFrame,
                   const glm::vec3& bgColor, const int& msaaSample) :
    windowWidth(windowWidth), windowHeight(windowHeight), bgColor(bgColor), secondPerFrame(secondPerFrame),
    accumulator(0.0f), msaaSample(msaaSample) {}

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

    // glEnable(GL_DEPTH_TEST);
    glEnable(GL_MULTISAMPLE);

    /***** DEBUG *****/
    shader = make_unique<ShaderParser>("shaders/geometry.vert", "shaders/geometry.frag");
    shader->use();
    shader->setVec3("color", glm::vec3(1.0f));
    glm::mat4 model;
    model = glm::translate(model, glm::vec3(windowWidth / 2.0f, windowHeight / 2.0f, 0.0f));
    model = glm::scale(model, glm::vec3(10.0f, 10.0f, 1.0f));
    shader->setMat4("model", model);
    shader->setMat4("projection", glm::ortho(0.0f, (float)windowWidth, 0.0f, (float)windowHeight, -1.0f, 1.0f));
    /***** DEBUG *****/

    return window;
}

void Renderer::initTimer() {
    curTime = SDL_GetTicks() * 0.001f;
}

void Renderer::bufferParticles(const uint32_t& VAO, glm::vec3 offsets[]) {
    /***** REFACTOR *****/
    // Initialize instanced array
    uint32_t instancedVBO;
    glGenBuffers(1, &instancedVBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, instancedVBO);
    glBufferData(GL_ARRAY_BUFFER, 100 * sizeof(glm::vec3), offsets, GL_STATIC_DRAW);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glVertexAttribDivisor(1, 1);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    /***** REFACTOR *****/
}

void Renderer::bufferParticles(glm::mat4 modelMats[], glm::vec3 colors[]) {}

void Renderer::setMSAASample(const int& sample) {
    if (msaaSample != sample) {
        msaaSample = sample;
        updateMSAA();
    }
}

void Renderer::clean() {
    SDL_GL_DeleteContext(glContext);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

/***** Private *****/

void Renderer::updateMSAA() {}
