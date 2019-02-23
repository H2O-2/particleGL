#pragma once

#include <vector>
#include <glm/glm.hpp>
#include <glad/glad.h>

// All Geometries are created at position (0,0,0) with radius / length of 1

class Geometry {
public:
    typedef std::vector<glm::vec2> TexCoord;
    typedef std::vector<glm::vec3> PosnCoord;
    typedef std::vector<uint32_t> IndexCoord;

    Geometry(const TexCoord& texCoords = TexCoord());
    virtual ~Geometry();

    virtual void init() = 0;
    virtual int getIndexNum() = 0;
    uint32_t getVAO();
protected:
    std::unique_ptr<uint32_t> VAO;
    uint32_t VBO, EBO;
    PosnCoord posns;
    IndexCoord indices;
    TexCoord texCoords;

    void bufferGeometry();
};
