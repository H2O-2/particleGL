#include <glad/glad.h>
#include "consoleMsg/consoleMsg.hpp"
#include "renderer.hpp"

#include <glm/gtc/matrix_transform.hpp>

Renderer::Renderer(unsigned int windowWidth, unsigned int windowHeight, float framerate, glm::vec3 bgColor)
                     : windowWidth(windowWidth), windowHeight(windowHeight), framerate(framerate), bgColor(bgColor) {
    test = std::unique_ptr<Sphere>(new Sphere());
    test2 = std::unique_ptr<Triangle>(new Triangle());
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

    int framebufferWidth, framebufferHeight;
    SDL_GL_GetDrawableSize(window, &framebufferWidth, &framebufferHeight);
    glViewport(0, 0, framebufferWidth, framebufferHeight);

    /***** DEBUG *****/
    glEnable(GL_DEPTH_TEST);
    test->init();
    test2->init();
    shader = std::unique_ptr<ShaderParser>(new ShaderParser("shaders/test.vert", "shaders/test.frag"));
    shader->use();
    shader->setVec3("color", glm::vec3(1.0f));
    glm::mat4 model;
    model = glm::translate(model, glm::vec3(windowWidth / 2.0f, windowHeight / 2.0f, 0.0f));
    model = glm::scale(model, glm::vec3(300.0f, 300.0f, 1.0f));
    shader->setMat4("model", model);
    shader->setMat4("projection", glm::ortho(0.0f, (float)windowWidth, 0.0f, (float)windowHeight, -1.0f, 1.0f));
    /***** DEBUG *****/

    return window;
}

void Renderer::renderEngine() {
    glClearColor(bgColor.x, bgColor.y, bgColor.z, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glBindVertexArray(test->getVAO());
    glDrawArrays(GL_TRIANGLE_FAN, 0, test->getSegmentNum() + 2);
    // glBindVertexArray(test2->getVAO());
    // glDrawArrays(GL_TRIANGLES, 0, 3);
    glBindVertexArray(0);
    SDL_GL_SwapWindow(window);
}

void Renderer::clean() {
    SDL_GL_DeleteContext(glContext);
    SDL_DestroyWindow(window);
    SDL_Quit();
}
