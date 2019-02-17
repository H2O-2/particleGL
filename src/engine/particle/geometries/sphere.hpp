#pragma once

#include <glm/glm.hpp>

class Sphere {
public:
    Sphere(glm::vec2 &posn, float radius, glm::vec3& color);
    Sphere(glm::vec3 &posn, float radius, glm::vec3& color);
private:
    float radius;
    glm::vec3 posn;
    glm::vec3 color;
};
