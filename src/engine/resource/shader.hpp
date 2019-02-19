#pragma once

#include <string>
#include <glm/glm.hpp>

#include "fileReader.hpp"

class Shader {
public:
    Shader();
    Shader(const string &vertexShaderPath, const string &fragShaderPath, const string &geometryShaderPath);
    uint32_t getID();
    void use();
    void setInt(const string &name, int v);
    void setFloat(const string &name, float v);
    void setBool(const string &name, bool v);
    void setVec2(const string &name, glm::vec2 v);
    void setVec3(const string &name, glm::vec3 v);
    void setMat4(const string &name, glm::mat4 v);
private:
    unsigned int id;
};
