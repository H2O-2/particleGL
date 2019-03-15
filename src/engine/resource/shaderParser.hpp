#pragma once

#include <string>
#include <map>
#include <glad/glad.h>
#include <glm/glm.hpp>

#include "fileReader.hpp"

class ShaderParser {
public:
    ShaderParser();
    ShaderParser(const string &vertexShaderPath, const string &fragShaderPath, const string &geometryShaderPath = string());
    ~ShaderParser();
    uint32_t getID() const;
    void init();
    void use();
    void setInt(const string &name, const int v);
    void setFloat(const string &name, const float v);
    void setBool(const string &name, const bool v);
    void setVec2(const string &name, const glm::vec2 &v);
    void setVec3(const string &name, const glm::vec3 &v);
    void setMat4(const string &name, const glm::mat4 &v);
private:
    uint32_t id;
    struct UniformVariable {
        int location = -1;
        int size;
        GLenum type;
    };
    std::map<string, UniformVariable> uniformBuffer;
    string vertexShaderPath;
    string fragShaderPath;
    string geometryShaderPath;

    int getUniformLocation(string name);
    void checkCompileError(uint32_t shader, GLenum type) const;
    void checkLinkError(uint32_t id) const;
};
