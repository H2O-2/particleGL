#pragma once

#include <glad/glad.h>

#include "fileReader.hpp"

class Texture {
public:
    Texture();
    Texture(const string& path, const GLenum internalFormat = GL_RGBA);
    ~Texture();
    void bind(const GLenum textureUnit = 0);
    uint32_t getID() const;
    float getAspectRatio() const;
    void setTexture(const string& path, const GLenum internalFormat = GL_RGBA);
    void setNullTexture();
    void setSize(const int width, const int height);
private:
    uint32_t id;
    int width;
    int height;
    int componentNum;

    void genTexture(unsigned char* textureData);
};
