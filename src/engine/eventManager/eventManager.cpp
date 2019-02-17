#include <glad/glad.h>
#include "eventManager.hpp"

bool EventManager::shouldQuit = false;

void EventManager::pollEvent(SDL_Window* window) {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT:
                shouldQuit = true;
                break;
            case SDL_WINDOWEVENT:
                switch (event.window.event) {
                    case SDL_WINDOWEVENT_CLOSE:
                        if (event.window.windowID == SDL_GetWindowID(window)) {
                            shouldQuit = true;
                        }
                        break;
                    case SDL_WINDOWEVENT_RESIZED:
                        int framebuffer_width, framebuffer_height;
                        SDL_GL_GetDrawableSize(window, &framebuffer_width, &framebuffer_height);
                        glViewport(0, 0, framebuffer_width, framebuffer_height);
                        break;
                    default:
                        break;
                }
                break;
            case SDL_KEYDOWN:
                switch (event.key.keysym.sym) {
                    case SDLK_q:
                        shouldQuit = true;
                        break;
                    default:
                        break;
                }
                break;
            default:
                break;
        }
    }
}