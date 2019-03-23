#pragma once

#include <glm/glm.hpp>

glm::vec3 colorLerp(const glm::vec3& c1, const glm::vec3& c2, float p) {
    return glm::mod(c1 * p + c2 * (1.0f - p), glm::vec3(1.0f));
}
