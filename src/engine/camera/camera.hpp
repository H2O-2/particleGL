#pragma once

#include <cmath>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

extern const glm::vec3 DEFAULT_POSN;
extern const glm::vec3 DEFAULT_DIR;
extern const glm::vec3 DEFAULT_RIGHT;
extern const glm::vec3 DEFAULT_UP;
extern const float DEFAULT_CAMERA_PITCH;
extern const float DEFAULT_CAMERA_YAW;
extern const float DEFAULT_MOUSE_SENSITIVITY;
extern const float DEFAULT_ZOOM;
extern const float PITCH_LIMIT;

class Camera {
public:
    Camera(glm::vec3 posn = DEFAULT_POSN);
    glm::mat4 getViewMatrix() const;
    void setCameraDirection(int xOffset, int yOffset);
    float getZoom() const;
    void setZoom(int offset);
private:
    glm::vec3 posn;
    glm::vec3 direction;
    glm::vec3 right;
    glm::vec3 up;
    float pitch;
    float yaw;

    float mouseSensitivity;
    float zoom;

    glm::mat4x4 viewMatrix;

    void update();
};
