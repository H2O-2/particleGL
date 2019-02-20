#include <glad/glad.h>
#include "consoleMsg/consoleMsg.hpp"
#include "engine/resource/shaderParser.hpp"
#include "renderer.hpp"

Renderer::Renderer(unsigned int windowWidth, unsigned int windowHeight, float framerate, glm::vec3 bgColor)
                     : windowWidth(windowWidth), windowHeight(windowHeight), framerate(framerate), bgColor(bgColor) {
    test = std::unique_ptr<Sphere>(new Sphere());
}

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

    int framebuffer_width, framebuffer_height;
    SDL_GetWindowSize(window, &framebuffer_width, &framebuffer_height);
    glViewport(0, 0, framebuffer_width, framebuffer_height);

    /***** DEBUG *****/
    test->init();
    ShaderParser();
    /***** DEBUG *****/

    return window;
}

void Renderer::renderEngine() {
    glDrawArrays(GL_TRIANGLE_FAN, 0, test->getSegmentNum() + 1);
    glClearColor(bgColor.x, bgColor.y, bgColor.z, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    SDL_GL_SwapWindow(window);
}

void Renderer::clean() {
    SDL_GL_DeleteContext(glContext);
    SDL_DestroyWindow(window);
    SDL_Quit();
}
