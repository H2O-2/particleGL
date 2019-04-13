#pragma once

#include <glm/glm.hpp>

glm::vec3 colorLerp(const glm::vec3& c1, const glm::vec3& c2, float p) {
    // If c1 and c2 represents the same color, skip interpolation
    if (glm::all(glm::equal(c1, c2))) return c1;

    return glm::mod(c1 * p + c2 * (1.0f - p), glm::vec3(1.0f));
}
