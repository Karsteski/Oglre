#include "Camera.h"

#include "glm/ext/matrix_transform.hpp"
#include "glm/ext/vector_float3.hpp"
#include "glm/fwd.hpp"
#include "glm/geometric.hpp"

// TODO: FIX THE ORDER OF THESE
// Camera variable definitions
glm::vec3 Oglre::Camera::cameraPosition(glm::vec3(200.0f, 200.0f, 400.0f));
glm::vec3 Oglre::Camera::cameraUp(glm::vec3(0.0f, 1.0f, 0.0f));
glm::vec3 Oglre::Camera::worldUp(glm::vec3(0.0f, 1.0f, 0.0f));
glm::vec3 Oglre::Camera::cameraRight(glm::vec3(0.0f, 0.0f, 0.0f));
glm::vec3 Oglre::Camera::cameraFront(glm::vec3(0.0f, 0.0f, -1.0f));
float Oglre::Camera::cameraPitch(defaultCameraPitch);
float Oglre::Camera::cameraYaw(defaultCameraYaw);
float Oglre::Camera::cameraSpeed(defaultCameraSpeed);
float Oglre::Camera::cameraSensitivity(defaultCameraSensitivity);
float Oglre::Camera::cameraFOV(defaultCameraFOV);
bool Oglre::Camera::constrainMovement(false);

Oglre::Camera::Camera()
{
    updateCameraVectors();
}

glm::mat4 Oglre::Camera::GetCameraViewMatrix()
{
    return glm::lookAt(cameraPosition, cameraPosition + cameraFront, cameraUp);
}

void Oglre::Camera::KeyboardInput(CameraMovements movement, float deltaTime)
{
    float cameraVelocity = cameraSpeed * deltaTime;

    if (!constrainMovement) {
        // The resulting right vectors are normalized as the camera speed would otherwise be based on the camera's orientation.
        switch (movement) {
        case CameraMovements::FORWARD: {
            cameraPosition += cameraVelocity * cameraFront;
            break;
        }
        case CameraMovements::BACKWARD: {
            cameraPosition -= cameraVelocity * cameraFront;
            break;
        }
        case CameraMovements::LEFT: {
            cameraPosition -= cameraVelocity * glm::normalize(glm::cross(cameraFront, cameraUp));
            break;
        }
        case CameraMovements::RIGHT: {
            cameraPosition += cameraVelocity * glm::normalize(glm::cross(cameraFront, cameraUp));
            break;
        }
        case CameraMovements::UP: {
            cameraPosition += cameraVelocity * cameraUp;
            break;
        }
        case CameraMovements::DOWN: {
            cameraPosition -= cameraVelocity * cameraUp;
            break;
        }
        }
    }
}

void Oglre::Camera::processMouseMovement(float xPositionOffset, float yPositionOffset)
{
    xPositionOffset *= cameraSensitivity;
    yPositionOffset *= cameraSensitivity;

    if (!constrainMovement) {
        cameraYaw += xPositionOffset;
        cameraPitch += yPositionOffset;
    }

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