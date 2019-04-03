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

    Geometry(const uint32_t& indexNum, const float& baseScale, GLenum drawMode = GL_TRIANGLES);
    virtual ~Geometry();

    virtual void init() = 0;
    int getIndexNum() const;
    GLenum getDrawMode() const;
    uint32_t getVAO() const;
    bool useEBO() const;
protected:
    uint32_t VAO, VBO, EBO;
    uint32_t indexNum; // Number of index of the geometry
    GLenum drawMode; // Kind of OpenGL primitive to render, e.g. GL_TRIANGLE_FAN
    float baseScale; // Offset for geometry's scale to match visual appearance in Particular
    IndexCoord indices; // Vertex indices for EBO
    PosnCoord posns; // Vertex positions
    TexCoord texCoords; // Texture Coordinates

    void bufferGeometry();
};
