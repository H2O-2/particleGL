#include "texture.hpp"

#include "stb_image.h"

Texture::Texture() : width(0), height(0) {}

Texture::Texture(const string& path, const GLenum internalFormat) {
    setTexture(path, internalFormat);
}

Texture::~Texture() {
    glDeleteTextures(1, &id);
}

void Texture::bind(const GLenum textureUnit) {
    if (textureUnit > 0)
        glActiveTexture(textureUnit);
    glBindTexture(GL_TEXTURE_2D, id);
}

uint32_t Texture::getID() const {
    return id;
}

float Texture::getAspectRatio() const {
    return static_cast<float>(width) / static_cast<float>(height);
}

void Texture::setTexture(const string& path, const GLenum internalFormat) {
    unsigned char* textureData = FileReader::readImage(path, &width, &height, &componentNum);
    // If texture data is invalid, skip texture generation and render squares instead
    if (!textureData) {
        stbi_image_free(textureData);
        return;
    }

    genTexture(textureData);

    stbi_image_free(textureData);
}

void Texture::setNullTexture() {
    componentNum = 3;
    genTexture(NULL);
}

void Texture::setSize(const int width, const int height) {
    this->width = width;
    this->height = height;
}


/***** Private *****/
void Texture::genTexture(unsigned char* textureData) {
    GLenum format = GL_RGBA;
    switch (componentNum) {
        case 1:
            format = GL_RED;
            break;
        case 3:
            format = GL_RGB;
            break;
        default:
            break;
    }

    glGenTextures(1, &id);
    glBindTexture(GL_TEXTURE_2D, id);
    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, textureData);
    glGenerateMipmap(GL_TEXTURE_2D);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, format == GL_RGBA ? GL_CLAMP_TO_EDGE : GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, format == GL_RGBA ? GL_CLAMP_TO_EDGE : GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glBindTexture(GL_TEXTURE_2D, 0);

}