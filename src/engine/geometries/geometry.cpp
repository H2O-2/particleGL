#include "geometry.hpp"
#include "consoleMsg/consoleMsg.hpp"

#include <string>
#include <memory>

Geometry::Geometry(const uint32_t& indexNum, const float& baseScale, const TexCoord& texCoords) :
    VAO(0), VBO(0), EBO(0), indexNum(indexNum), baseScale(baseScale), texCoords(texCoords) {}

Geometry::~Geometry() {
    glDeleteBuffers(1, &VBO);
    if (indices.size()) {
        glDeleteBuffers(1, &EBO);
    }
    glDeleteVertexArrays(1, &VAO);
}

float Geometry::getBaseScale() {
    return baseScale;
}

uint32_t Geometry::getVAO() {
    return VAO;
}

void Geometry::bufferGeometry() {
    // Initialize vertex data array
    std::vector<float> vertexData;
    vertexData.reserve(posns.size() + texCoords.size());

    for (int i = 0; i < posns.size(); ++i) {
        vertexData.push_back(posns[i].x);
        vertexData.push_back(posns[i].y);
        vertexData.push_back(posns[i].z);

        if (texCoords.size()) {
            vertexData.push_back(texCoords[i].x);
            vertexData.push_back(texCoords[i].y);
        }
    }

    // Configure VAO and VBO
    if (!VAO) {
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
    }

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertexData.size() * sizeof(float), vertexData.data(), GL_STATIC_DRAW);

    // Configure EBO only if an index array is provided
    if (indices.size()) {
        glGenBuffers(1, &EBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(uint32_t), indices.data(),GL_STATIC_DRAW);
    }

    int stride = 3 * sizeof(float) + (texCoords.size() > 0 ? texCoords.size() * sizeof(float) : 0);

    int offset = 0;
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*)0);
    glEnableVertexAttribArray(0);

    if (texCoords.size() > 0) {
        offset += posns.size() * sizeof(float);
        glVertexAttribPointer(1, texCoords.size(), GL_FLOAT, GL_FALSE, stride, (void*)offset);
        glEnableVertexAttribArray(1);
    }

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}
