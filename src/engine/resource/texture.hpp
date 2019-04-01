#pragma once

#include <glad/glad.h>

#include "fileReader.hpp"

class Texture {
public:
    Texture(const string& path, const GLenum internalFormat = GL_RGBA);
    ~Texture();
    void bind(const GLenum textureUnit);
    float getAspectRatio() const;
    void setTexture(const string& path, const GLenum internalFormat = GL_RGBA);
private:
    uint32_t id;
    int width;
    int height;
    int componentNum;
};
