#include "sphere.hpp"

Sphere::Sphere(glm::vec2 &posn, float radius, glm::vec3& color) : posn(glm::vec3(posn, 0.0f)), radius(radius), color(color) {
    
}

Sphere::Sphere(glm::vec3 &posn, float radius, glm::vec3& color) {}
