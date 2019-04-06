#pragma once

#include <glad/glad.h>

#include "engine/resource/texture.hpp"

// Very simple wrapper for framebuffer with one color buffer

class RenderBuffer {
public:
    RenderBuffer();
    ~RenderBuffer();
    void init(int width, int height, bool requireDepthStencil = false);
    void bind();
    void bindColorBuffer(const GLenum textureUnit);
private:
    uint32_t id;
    Texture colorTexture;
};
