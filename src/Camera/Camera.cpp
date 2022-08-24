#include "Camera.h"

#include "glm/ext/matrix_transform.hpp"
#include "glm/ext/vector_float3.hpp"
#include "glm/fwd.hpp"
#include "glm/geometric.hpp"


Oglre::Camera::Camera()
{
    updateCameraVectors();
}

glm::mat4 Oglre::Camera::GetCameraViewMatrix()
{
    return glm::lookAt(cameraPosition, cameraPosition + cameraFront, cameraUp);
}

bool Oglre::moveCamera(Camera camera, CameraMovement movement){
    float cameraVelocity = camera.cameraSpeed  ; // * deltaTime

    if (!camera.constrainMovement) {
        // The resulting right vectors are normalized as the camera speed would otherwise be based on the camera's orientation.
        switch (movement) {
        case CameraMovement::FORWARD: {
            camera.cameraPosition += cameraVelocity * camera.cameraFront;
            break;
        }
        case CameraMovement::BACKWARD: {
            camera.cameraPosition -= cameraVelocity * camera.cameraFront;
            break;
        }
        case CameraMovement::LEFT: {
            camera.cameraPosition -= cameraVelocity * glm::normalize(glm::cross(camera.cameraFront, camera.cameraUp));
            break;
        }
        case CameraMovement::RIGHT: {
            camera.cameraPosition += cameraVelocity * glm::normalize(glm::cross(camera.cameraFront, camera.cameraUp));
            break;
        }
        case CameraMovement::UP: {
            camera.cameraPosition += cameraVelocity * camera.cameraUp;
            break;
        }
        case CameraMovement::DOWN: {
            camera.cameraPosition -= cameraVelocity * camera.cameraUp;
            break;
        }
        }

        return true;
    }

    return false;
}

void Oglre::Camera::KeyboardInput(CameraMovement movement, float deltaTime)
{
    // To refactor KeyboardInput.
    // - Move Input stuff into free functions
    // - Have camera just be set of positions and direction
    // - Have free function to move a camera.
    // - Have a function that has a static time variable, then updates on time passed since last call i.e. in render loop
    float cameraVelocity = cameraSpeed * deltaTime;

    if (!constrainMovement) {
        // The resulting right vectors are normalized as the camera speed would otherwise be based on the camera's orientation.
        switch (movement) {
        case CameraMovement::FORWARD: {
            cameraPosition += cameraVelocity * cameraFront;
            break;
        }
        case CameraMovement::BACKWARD: {
            cameraPosition -= cameraVelocity * cameraFront;
            break;
        }
        case CameraMovement::LEFT: {
            cameraPosition -= cameraVelocity * glm::normalize(glm::cross(cameraFront, cameraUp));
            break;
        }
        case CameraMovement::RIGHT: {
            cameraPosition += cameraVelocity * glm::normalize(glm::cross(cameraFront, cameraUp));
            break;
        }
        case CameraMovement::UP: {
            cameraPosition += cameraVelocity * cameraUp;
            break;
        }
        case CameraMovement::DOWN: {
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
