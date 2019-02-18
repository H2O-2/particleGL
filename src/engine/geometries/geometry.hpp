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
protected:
    uint32_t VAO, VBO, EBO;
    PosnCoord posns;
    IndexCoord indices;
    TexCoord texCoords;

    void bufferGeometry();
};
