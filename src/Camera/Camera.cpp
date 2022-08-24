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
    return glm::lookAt(m_cameraPosition, m_cameraPosition + m_cameraFront, m_cameraUp);
}

bool Oglre::Camera::moveCamera( CameraMovement movement){
    float cameraVelocity = m_cameraSpeed  ; // * deltaTime

    if (!m_constrainMovement) {
        // The resulting right vectors are normalized as the camera speed would otherwise be based on the camera's orientation.
        switch (movement) {
        case CameraMovement::FORWARD: {
            m_cameraPosition += cameraVelocity * m_cameraFront;
            break;
        }
        case CameraMovement::BACKWARD: {
            m_cameraPosition -= cameraVelocity * m_cameraFront;
            break;
        }
        case CameraMovement::LEFT: {
            m_cameraPosition -= cameraVelocity * glm::normalize(glm::cross(m_cameraFront, m_cameraUp));
            break;
        }
        case CameraMovement::RIGHT: {
            m_cameraPosition += cameraVelocity * glm::normalize(glm::cross(m_cameraFront, m_cameraUp));
            break;
        }
        case CameraMovement::UP: {
            m_cameraPosition += cameraVelocity * m_cameraUp;
            break;
        }
        case CameraMovement::DOWN: {
            m_cameraPosition -= cameraVelocity * m_cameraUp;
            break;
        }
        }

        return true;
    }

    return false;
}

bool Oglre::Camera::rotateCamera(float x_axis_rotate_degrees, float y_axis_rotate_degrees){

}
bool Oglre::Camera::zoomCamera(float adjust_FOV){

}

void Oglre::Camera::KeyboardInput(CameraMovement movement, float deltaTime)
{
    // To refactor KeyboardInput.
    // - Move Input stuff into free functions
    // - Have camera just be set of positions and direction
    // - Have free function to move a camera.
    // - Have a function that has a static time variable, then updates on time passed since last call i.e. in render loop
    float cameraVelocity = m_cameraSpeed * deltaTime;

    if (!m_constrainMovement) {
        // The resulting right vectors are normalized as the camera speed would otherwise be based on the camera's orientation.
        switch (movement) {
        case CameraMovement::FORWARD: {
            m_cameraPosition += cameraVelocity * m_cameraFront;
            break;
        }
        case CameraMovement::BACKWARD: {
            m_cameraPosition -= cameraVelocity * m_cameraFront;
            break;
        }
        case CameraMovement::LEFT: {
            m_cameraPosition -= cameraVelocity * glm::normalize(glm::cross(m_cameraFront, m_cameraUp));
            break;
        }
        case CameraMovement::RIGHT: {
            m_cameraPosition += cameraVelocity * glm::normalize(glm::cross(m_cameraFront, m_cameraUp));
            break;
        }
        case CameraMovement::UP: {
            m_cameraPosition += cameraVelocity * m_cameraUp;
            break;
        }
        case CameraMovement::DOWN: {
            m_cameraPosition -= cameraVelocity * m_cameraUp;
            break;
        }
        }
    }
}

void Oglre::Camera::processMouseMovement(float xPositionOffset, float yPositionOffset)
{
    xPositionOffset *= m_cameraSensitivity;
    yPositionOffset *= m_cameraSensitivity;

    if (!m_constrainMovement) {
        m_cameraYaw += xPositionOffset;
        m_cameraPitch += yPositionOffset;
    }

    // Constrain pitch because at 90 degrees the LookAt function flips the camera direction.
    if (m_cameraPitch > 89.0f) {
        m_cameraPitch = 89.0f;
    }
    if (m_cameraPitch < -89.0f) {
        m_cameraPitch = -89.0f;
    }

    updateCameraVectors();
}

void Oglre::Camera::processMouseScroll(float yPositionOffset)
{
    m_cameraFOV -= static_cast<float>(yPositionOffset);

    // Constrain zoom/FOV values.
    if (m_cameraFOV < 1.0f) {
        m_cameraFOV = 1.0f;
    }
    if (m_cameraFOV > 90.0f) {
        m_cameraFOV = 90.0f;
    }
}

void Oglre::Camera::updateCameraVectors()
{
    // Create camera direction vector using Euler angles.
    glm::vec3 cameraDirection;

    // Must convert to radians first.
    // Note that xz sides are influenced by cos(pitch) and must therefore be included in their calculations.
    cameraDirection.x = std::cos(glm::radians(m_cameraYaw)) * std::cos(glm::radians(m_cameraPitch));
    cameraDirection.y = sin(glm::radians(m_cameraPitch));
    cameraDirection.z = std::sin(glm::radians(m_cameraYaw)) * std::cos(glm::radians(m_cameraPitch));

    m_cameraFront = glm::normalize(cameraDirection);
    m_cameraRight = glm::normalize(glm::cross(m_cameraFront, m_worldUp));
    m_cameraUp = glm::normalize(glm::cross(m_cameraRight, m_cameraFront));
}
