#pragma once

#include <vector>
#include <memory>
#include <glm/glm.hpp>
#include <glad/glad.h>

// All Geometries are created at position (0,0,0) with radius / length of 1

class Geometry {
public:
    typedef std::vector<glm::vec2> TexCoord;
    typedef std::vector<glm::vec3> PosnCoord;
    typedef std::vector<uint32_t> IndexCoord;

    Geometry(const uint32_t& indexNum, const float& baseScale = 1.0f, const TexCoord& texCoords = TexCoord(), const IndexCoord& indexCoords = IndexCoord());
    virtual ~Geometry();

    virtual void init() = 0;
    virtual int getIndexNum() = 0;
    float getBaseScale();
    uint32_t getVAO();
protected:
    uint32_t VAO, VBO, EBO;
    uint32_t indexNum; // Number of index of the geometry
    float baseScale; // Offset for geometry's scale to match visual appearance in Particular
    PosnCoord posns; // Vertex positions
    IndexCoord indices; // Vertex indices for EBO
    TexCoord texCoords; // Texture Coordinates

    void bufferGeometry();
};
