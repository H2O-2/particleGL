#pragma once

class Shader {
public:
    Shader();
    Shader(const char* vertexShaderPath, const char* fragShaderPath, const char* geometryShaderPath);
    unsigned int getID();
    void use();
private:
    unsigned int id;
};
