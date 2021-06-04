#include "Camera.h"

#include "glm/ext/matrix_transform.hpp"
#include "glm/ext/vector_float3.hpp"
#include "glm/fwd.hpp"
#include "glm/geometric.hpp"

Oglre::Camera::Camera(glm::vec3 positionVector, glm::vec3 upVector, float yaw, float pitch)
    : cameraFront(glm::vec3(0.0f, 0.0f, -1.0f))
    , cameraPitch(defaultCameraPitch)
    , cameraYaw(defaultCameraYaw)
    , cameraSpeed(defaultCameraSpeed)
    , cameraSensitivity(defaultCameraSensitivity)
    , cameraFOV(defaultCameraFOV)
{
    cameraPosition = positionVector;
    worldUp = upVector;

    cameraYaw = yaw;
    cameraPitch = pitch;

    updateCameraVectors();
}

glm::mat4 Oglre::Camera::GetCameraViewMatrix()
{
    return glm::lookAt(cameraPosition, cameraPosition + cameraFront, cameraUp);
}

void Oglre::Camera::processKeyboardInput(CameraMovements movement, float deltaTime)
{
    float cameraVelocity = cameraSpeed * deltaTime;

    switch (movement) {
    case CameraMovements::FORWARD: {
        cameraPosition += cameraVelocity * cameraFront;
    }
    case CameraMovements::BACKWARD: {
        cameraPosition -= cameraVelocity * cameraFront;
    }
    case CameraMovements::LEFT: {
        cameraPosition -= cameraVelocity * glm::normalize(glm::cross(cameraFront, cameraUp));
    }
    case CameraMovements::RIGHT: {
        cameraPosition += cameraVelocity * glm::normalize(glm::cross(cameraFront, cameraUp));
    }
    case CameraMovements::UP: {
        cameraPosition += cameraVelocity * cameraUp;
    }
    case CameraMovements::DOWN: {
        cameraPosition -= cameraVelocity * cameraUp;
    }
    }
}

void Oglre::Camera::processMouseMovement(float xPositionOffset, float yPositionOffset)
{
    xPositionOffset *= cameraSensitivity;
    yPositionOffset *= cameraSensitivity;

    cameraYaw += xPositionOffset;
    cameraPitch += yPositionOffset;

    // Constrain pitch because at 90 degrees the LookAt function flips the camera direction.
    if (cameraPitch > 89.0f) {
        cameraPitch = 89.0f;
    }
    if (cameraPitch < -89.0f) {
        cameraPitch = -89.0f;
    }

    updateCameraVectors();
}

void Oglre::Camera::processMouseScroll(float yPositionOffset)
{
    cameraFOV -= static_cast<float>(yPositionOffset);

    // Constrain zoom/FOV values.
    if (cameraFOV < 1.0f) {
        cameraFOV = 1.0f;
    }
    if (cameraFOV > 90.0f) {
        cameraFOV = 90.0f;
    }
}

void Oglre::Camera::updateCameraVectors()
{
    // Create camera direction vector using Euler angles.
    glm::vec3 cameraDirection;

    // Must convert to radians first.
    // Note that xz sides are influenced by cos(pitch) and must therefore be included in their calculations.
    cameraDirection.x = std::cos(glm::radians(cameraYaw)) * std::cos(glm::radians(cameraPitch));
    cameraDirection.y = sin(glm::radians(cameraPitch));
    cameraDirection.z = std::sin(glm::radians(cameraYaw)) * std::cos(glm::radians(cameraPitch));

    cameraFront = glm::normalize(cameraDirection);
    cameraRight = glm::normalize(glm::cross(cameraFront, worldUp));
    cameraUp = glm::normalize(glm::cross(cameraRight, cameraFront));
}