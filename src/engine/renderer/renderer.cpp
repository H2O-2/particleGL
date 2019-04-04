#include <glad/glad.h>
#include "renderer.hpp"
#include "../../util/makeUnique.hpp"
#include "controlGUI/controlGUI.hpp"

const int DEFAULT_MSAA = 8;
const float PLANE_SCALE = 0.1f;
const float DEFAULT_NEAR_PLANE = 1.0f;
const float DEFAULT_FAR_PLANE = 20000.0f;
const ParticleBlend INIT_BLEND_TYPE = ParticleBlend::NORMAL;
const float INIT_COLOR_BLEND = 1.0f;

const int Renderer::OFFSET_POSN = 2;
const int Renderer::COLOR_POSN = 3;
const int Renderer::MODEL_MAT_POSN = 4;

Renderer::Renderer() {}

Renderer::Renderer(const uint32_t windowWidth, const uint32_t windowHeight, const float secondPerFrame,
                   const glm::vec3& bgColor, const int msaaSample) :
    bgColor(bgColor), blendType(INIT_BLEND_TYPE), prevBlendType(INIT_BLEND_TYPE), colorBlend(INIT_COLOR_BLEND),
    currentRenderMode(DEFAULT_RENDER), msaaSample(msaaSample), secondPerFrame(secondPerFrame),
    windowWidth(windowWidth), windowHeight(windowHeight),
    nearVanish(DEFAULT_NEAR_PLANE * PLANE_SCALE), farVanish(DEFAULT_FAR_PLANE * PLANE_SCALE), paused(false),
    screenShader(ShaderParser("shaders/screen.vert", "shaders/screen.frag")) {}

Renderer::~Renderer() {
    ControlGUI::destroy();
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

    glEnable(GL_BLEND);
    glBlendFuncSeparate(GL_ONE, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ZERO);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_MULTISAMPLE);

    screenQuad = Square(static_cast<float>(framebufferWidth) / static_cast<float>(framebufferHeight), true);
    screenQuad.init();
    particleFBO.init(framebufferWidth, framebufferHeight);

    // Initialize GUI
    ControlGUI::init(window, glContext, isHidpi());

    return window;
}

void Renderer::initTimer() {
    curTime = SDL_GetTicks() * 0.001f;
}

void Renderer::initShader(const Camera& camera) {
    shaders.emplace(RenderMode::U_MODEL_U_COLOR, ShaderPair({{true, ShaderParser("shaders/geometry/geoUniformModelUniformColor.vert", "shaders/texture/textureUniformColor.frag")}, {false, ShaderParser("shaders/geometry/geoUniformModelUniformColor.vert", "shaders/geometry/geoUniformColor.frag")}}));
    shaders.emplace(RenderMode::U_MODEL_V_COLOR, ShaderPair({{true, ShaderParser("shaders/geometry/geoUniformModelVaringColor.vert", "shaders/texture/textureVaringColor.frag")}, {false, ShaderParser("shaders/geometry/geoUniformModelVaringColor.vert", "shaders/geometry/geoVaringColor.frag")}}));
    shaders.emplace(RenderMode::V_MODEL_U_COLOR, ShaderPair({{true, ShaderParser("shaders/geometry/geoVaringModelUniformColor.vert", "shaders/texture/textureUniformColor.frag")}, {false, ShaderParser("shaders/geometry/geoVaringModelUniformColor.vert", "shaders/geometry/geoUniformColor.frag")}}));
    shaders.emplace(RenderMode::V_MODEL_V_COLOR, ShaderPair({{true, ShaderParser("shaders/geometry/geoVaringModelVaringColor.vert", "shaders/texture/textureVaringColor.frag")}, {false, ShaderParser("shaders/geometry/geoVaringModelVaringColor.vert", "shaders/geometry/geoVaringColor.frag")}}));

    // Initialize particle shaders and use the default one
    for (auto& shaderPair : shaders) {
        for (auto& shader : shaderPair.second) {
            shader.second.init();
        }
    }

    // Initialize screen shader
    screenShader.init();
    screenShader.setInt("frameBuffer", 0);
}

void Renderer::initParticleBuffer(const uint32_t VAO) {
    glBindVertexArray(VAO);

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

void Renderer::renderEngine(const std::vector<std::shared_ptr<Emitter>>& emitters, const Camera& camera) {
    // Referenced from https://gafferongames.com/post/fix_your_timestep/
    float accumulator = 0.0f;
    float newTime = SDL_GetTicks() * 0.001f;
    float deltaTime = newTime - curTime;
    curTime = newTime;

    accumulator += deltaTime;

    while (accumulator >= secondPerFrame) {
        updateParticleStatus(emitters, 1.0f, paused); // update with one full frame
        accumulator -= secondPerFrame;
    }

    // Update render mode
    updateCurrentRenderMode(emitters);

    // Update blend mode
    updateBlendMode();

    // Calculate the portion of the partial frame left in the accumulator and update
    const float interpolation = accumulator / secondPerFrame;
    updateParticleStatus(emitters, interpolation, paused);

    particleFBO.bind();
    clearScreen();

    // Render GUI
    renderGUI(emitters);

    // Render particles
    for (uint32_t i = 0; i < emitters.size(); ++i) {
        auto const& emitter = emitters[i];

        bool isSprite = (emitter->getParticleType() == ParticleType::SPRITE);

        ShaderParser& shader = shaders[currentRenderMode][isSprite];
        shader.use();
        shader.setMat4("view", camera.getViewMatrix());
        shader.setMat4("projection", glm::perspective(glm::radians(camera.getZoom()), (float)windowWidth / (float)windowHeight, nearVanish, farVanish));

        if (isSprite) {
            shader.setFloat("colorBlend", colorBlend);
            shader.setInt("particleTexture", i);
            emitter->bindTexture(GL_TEXTURE0 + i);
        }

        glBindVertexArray(emitter->getVAO());

        // Update the transformation & color buffers for particles
        // i.e. update instanced vertex attributes (buffers) and uniform variables
        glm::mat4 particleModel;
        switch (currentRenderMode) {
            case RenderMode::U_MODEL_U_COLOR:
                updateParticleBuffer(emitter->getOffsets());

                particleModel = glm::scale(particleModel, glm::vec3(emitter->getParticleSize()));
                shader.setMat4("particleModel", particleModel);
                shader.setVec4("color", emitter->getParticleColorAndOpacity());
                break;
            case RenderMode::U_MODEL_V_COLOR:
                updateParticleBuffer(emitter->getOffsets(), emitter->getInstancedColors());

                particleModel = glm::scale(particleModel, glm::vec3(emitter->getParticleSize()));
                shader.setMat4("particleModel", particleModel);
                break;
            case RenderMode::V_MODEL_U_COLOR:
                updateParticleBufferWithMatrices(emitter->getModelMatrices());

                shader.setVec4("color", emitter->getParticleColorAndOpacity());
                break;
            case RenderMode::V_MODEL_V_COLOR:
                updateParticleBufferWithMatrices(emitter->getModelMatrices(), emitter->getInstancedColors());
                break;
            default:
                break;
        }

        // Render particles
        if (emitter->useEBO()) {
            glDrawElementsInstanced(emitter->getDrawMode(), emitter->getIndexNum(), GL_UNSIGNED_INT, 0, emitter->getCurrentParticleNum());
        } else {
            glDrawArraysInstanced(emitter->getDrawMode(), 0, emitter->getIndexNum(), emitter->getCurrentParticleNum());
        }
        glBindVertexArray(0);
    }

    // Bind default framebuffer and clear buffer
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    clearScreen();
    screenShader.use();

    // Draw framebuffer to screen
    glBindVertexArray(screenQuad.getVAO());
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, screenQuad.getEBO());
    particleFBO.bindColorBuffer(GL_TEXTURE0);
    glDrawElements(screenQuad.getDrawMode(), screenQuad.getIndexNum(), GL_UNSIGNED_INT, 0);

    SDL_GL_SwapWindow(window);
}

/***** Private *****/

bool Renderer::isHidpi() const {
    return display.w > 2048;
}


void Renderer::clearScreen() {
    glClearColor(bgColor.x, bgColor.y, bgColor.z, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}


void Renderer::renderGUI(const std::vector<std::shared_ptr<Emitter>>& emitters) {
    ControlGUI::preRender(window);
    ParticleType newParticleType;
    EmitterDirection emitterDirectionType;
    EmitterSize emitterSizeType;
    EmitterType emitterType;
    for (auto& emitter : emitters) {
        newParticleType = emitter->getParticleType();
        emitterDirectionType = emitter->getEmitterDirectionType();
        emitterSizeType = emitter->getEmitterSizeType();
        emitterType = emitter->getEmitterType();

        if (ControlGUI::renderMenu("Emitter (Master)")) {
            ControlGUI::renderIntSlider("Particles/sec", (int *)emitter->getParticlesPerSecPtr(), 0, 1000);
            ControlGUI::renderPullDownMenu("Emitter Type", {"Point", "Box", "Sphere"}, emitter->getEmitterTypePtr());
            ControlGUI::render3dFloatSlider("Position", emitter->getEmitterPosnPtr());
            ControlGUI::renderPullDownMenu("Direction", {"Uniform", "Directional"}, emitter->getEmitterDirectionTypePtr());
            if (emitterDirectionType != EmitterDirection::UNIFORM)
                ControlGUI::renderFloatSlider("Direction Spread [%%]", emitter->getEmitterDirectionSpreadPtr(), 0, 100, PERCENTAGE_SCALE);
            ControlGUI::render3dFloatSlider("Rotation (°)", emitter->getEmitterRotationPtr());
            ControlGUI::renderFloatSlider("Velocity", emitter->getInitialVelocityPtr(), 0.0f, 1000.0f, PARTICLE_VELOCITY_SCALE);
            ControlGUI::renderFloatSlider("Velocity Random [%%]", emitter->getInitialVelocityRandomnessPtr(), 0, 100, PERCENTAGE_SCALE);
            ControlGUI::renderFloatSlider("Velocity Distribution", emitter->getInitialVelocityRandomnessDistributionPtr(), 0.0f, 1.0);

            if (emitterType != EmitterType::POINT) {
                ControlGUI::renderPullDownMenu("Emitter Size", {"XYZ Linked", "XYZ Individual"}, emitter->getEmitterSizeTypePtr());

                if (emitterSizeType == EmitterSize::LINKED) {
                    ControlGUI::renderUnsignedIntDragger("Emitter Size XYZ", emitter->getEmitterSizePtr(), 3, EMITTER_SIZE_SCALE);
                } else {
                    ControlGUI::render3dUnsignedIntSlider("Emitter Size XYZ", emitter->getEmitterSizePtr(), EMITTER_SIZE_SCALE);
                }
            }
        }

        if (ControlGUI::renderMenu("Particle (Master)")) {
            ControlGUI::renderPullDownMenu("Particle Type", {"Sphere", "Square", "Triangle", "Sprite"}, &(newParticleType));
            if (newParticleType == ParticleType::SPRITE)
                ControlGUI::renderTextInput("Sprite Path", emitter->getParticleTexturePathPtr());
            ControlGUI::renderFloatDragger("Life [sec]", emitter->getParticleLifePtr(), 1, 0.05f);
            ControlGUI::renderFloatSlider("Life Random [%%]", emitter->getParticleLifeRandomnessPtr(), 0, 100, PERCENTAGE_SCALE);
            if (newParticleType == ParticleType::SQUARE || newParticleType == ParticleType::SPRITE)
                ControlGUI::renderFloatSlider("Aspect Ratio", emitter->getParticleAspectRatioPtr(), 0.0f, 10.0f, 1.0f, "%.2f");
            ControlGUI::renderFloatSlider("Size", emitter->getParticleSizePtr(), 0.0f, 100.0f, PARTICLE_SIZE_SCALE);
            ControlGUI::renderFloatSlider("Size Random [%%]", emitter->getParticleSizeRandomnessPtr(), 0, 100, PERCENTAGE_SCALE);
            ControlGUI::renderFloatSlider("Opacity", emitter->getParicleOpacityPtr(), 0.0f, 100.0f, PERCENTAGE_SCALE);
            ControlGUI::renderFloatSlider("Opacity Random [%%]", emitter->getParicleOpacityRandomnessPtr(), 0, 100, PERCENTAGE_SCALE);
            ControlGUI::renderColorEdit3("Color", emitter->getParticleColorPtr());
            ControlGUI::renderIntSlider("Color Random", emitter->getParticleColorRandomnessPtr(), 0, 100, PERCENTAGE_SCALE);
            if (newParticleType == ParticleType::SPRITE)
                ControlGUI::renderFloatSlider("Color Blend", &colorBlend, 0.0f, 1.0f);
            ControlGUI::renderPullDownMenu("Blend Mode", {"Normal", "Add", "Screen", "Lighten"}, &blendType);
        }

        if (ControlGUI::renderMenu("Physics (Master)")) {

        }

        if (ControlGUI::renderMenu("Visibility")) {

        }

        if (ControlGUI::renderMenu("Rendering")) {

        }

        ControlGUI::renderCheckbox("Pause", &paused);

        emitter->setParticleType(newParticleType);
    }
    ControlGUI::finalizeRender();
}

void Renderer::updateBlendMode() {
    if (prevBlendType == blendType) return;

    if (prevBlendType == ParticleBlend::LIGHTEN)
        glBlendEquation(GL_FUNC_ADD);

    switch (blendType) {
        case ParticleBlend::NORMAL:
            glBlendFuncSeparate(GL_ONE, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ZERO);
            break;
        case ParticleBlend::ADD:
            glBlendFuncSeparate(GL_ONE, GL_ONE, GL_ONE, GL_ZERO);
            break;
        case ParticleBlend::SCREEN:
            glBlendFuncSeparate(GL_ONE, GL_ONE_MINUS_SRC_COLOR, GL_ONE, GL_ZERO);
            break;
        case ParticleBlend::LIGHTEN:
            glBlendEquationSeparate(GL_MAX, GL_FUNC_ADD);
            glBlendFuncSeparate(GL_ONE, GL_ONE, GL_ONE, GL_ZERO);
            break;
        default:
            break;
    }

    prevBlendType = blendType;
}

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

void Renderer::updateParticleStatus(const std::vector<std::shared_ptr<Emitter>>& emitters, const float interpolation, const bool paused) const {
    if (paused) return;

    for (auto const& emitter : emitters) {
        emitter->update(interpolation);
    }
}

void Renderer::updateParticleBuffer(const std::vector<float>& offsets) {
    glBindBuffer(GL_ARRAY_BUFFER, instancedOffsetVBO);
    glBufferSubData(GL_ARRAY_BUFFER, 0, offsets.size() * sizeof(float), offsets.data());
    glEnableVertexAttribArray(OFFSET_POSN);
    glVertexAttribPointer(OFFSET_POSN, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glVertexAttribDivisor(OFFSET_POSN, 1);
}

void Renderer::updateParticleBuffer(const std::vector<float>& offsets, const std::vector<float>& colors) {
    updateParticleBuffer(offsets);

    glBindBuffer(GL_ARRAY_BUFFER, instancedColorVBO);
    glBufferSubData(GL_ARRAY_BUFFER, 0, colors.size() * sizeof(float), colors.data());
    glEnableVertexAttribArray(COLOR_POSN);
    glVertexAttribPointer(COLOR_POSN, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glVertexAttribDivisor(COLOR_POSN, 1);
}

void Renderer::updateParticleBufferWithMatrices(const std::vector<float>& modelMatrices) {
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

void Renderer::updateParticleBufferWithMatrices(const std::vector<float>& modelMatrices, const std::vector<float>& colors) {
    updateParticleBufferWithMatrices(modelMatrices);

    glBindBuffer(GL_ARRAY_BUFFER, instancedColorVBO);
    glBufferSubData(GL_ARRAY_BUFFER, 0, colors.size() * sizeof(float), colors.data());
    glEnableVertexAttribArray(COLOR_POSN);
    glVertexAttribPointer(COLOR_POSN, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glVertexAttribDivisor(COLOR_POSN, 1);
}

void Renderer::updateMSAA() {}
