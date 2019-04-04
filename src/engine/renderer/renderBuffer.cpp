#include "renderBuffer.hpp"

#include "consoleMsg/consoleMsg.hpp"

RenderBuffer::RenderBuffer() {}

void RenderBuffer::init(int width, int height, bool requireDepthStencil) {

    glGenFramebuffers(1, &id);
    glBindFramebuffer(GL_FRAMEBUFFER, id);

    colorTexture.setSize(width, height);
    colorTexture.setNullTexture();
    colorTexture.bind();
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, colorTexture.getID(), 0);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        ConsoleMsg::errorMsg("FRAMEBUFFER INCOMPLETE");
        exit(1);
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

RenderBuffer::~RenderBuffer() {
    glDeleteFramebuffers(1, &id);
}

void RenderBuffer::bind() {
    glBindFramebuffer(GL_FRAMEBUFFER, id);
}

void RenderBuffer::bindColorBuffer(const GLenum textureUnit) {
    colorTexture.bind(textureUnit);
}
