#include "camera.hpp"

#include <cmath>

const glm::vec3 DEFAULT_POSN(0.0f, 0.0f, 200.0f);
const glm::vec3 DEFAULT_DIR(0.0f, 0.0f, -1.0f);
const glm::vec3 DEFAULT_RIGHT(1.0f, 0.0f, 0.0f);
const glm::vec3 DEFAULT_UP(0.0f, 1.0f, 0.0f);
const float DEFAULT_CAMERA_PITCH = 0.0f;
const float DEFAULT_CAMERA_YAW = -90.0f;
const float DEFAULT_MOUSE_SENSITIVITY = 0.1f;
const float DEFAULT_ZOOM = 45.0f;
const float PITCH_LIMIT = std::nextafter(90.0f, 0.0f);

Camera::Camera(glm::vec3 posn) : posn(posn), direction(DEFAULT_DIR),
    right(DEFAULT_RIGHT), up(DEFAULT_UP), pitch(DEFAULT_CAMERA_PITCH), yaw(DEFAULT_CAMERA_YAW),
    mouseSensitivity(DEFAULT_MOUSE_SENSITIVITY), zoom(DEFAULT_ZOOM) {
        viewMatrix = glm::lookAt(posn, posn + direction, up);
    }

glm::mat4 Camera::getViewMatrix() const {
    return viewMatrix;
}

void Camera::setCameraDirection(int xOffset, int yOffset) {
    yaw += xOffset * mouseSensitivity;
    pitch += yOffset * mouseSensitivity;

    if (pitch > PITCH_LIMIT) {
        pitch = PITCH_LIMIT;
    } else if (pitch < -PITCH_LIMIT) {
        pitch = -PITCH_LIMIT;
    }

    update();
}

float Camera::getZoom() const {
    return zoom;
}

void Camera::setZoom(int offset) {
    if (zoom >= 1.0f && zoom < 90.0f)
        zoom -= offset;
    if (zoom < 1.0f)
        zoom = 1.0f;
    if (zoom >= 90.0f)
        zoom = std::nextafter(90.0f, std::numeric_limits<float>::min());
}


/***** Private *****/

void Camera::update() {
    float horizontal = cosf(glm::radians(pitch));
    direction = glm::vec3(horizontal * cosf(glm::radians(yaw)), sinf(glm::radians(pitch)), horizontal * sinf(glm::radians(yaw)));
    right = glm::normalize(glm::cross(direction, glm::vec3(0.0f, 1.0f, 0.0f)));
    up = glm::cross(right, direction);

    viewMatrix = glm::lookAt(posn, posn + direction, up);
}
