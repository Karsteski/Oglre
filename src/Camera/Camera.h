#pragma once

#include "glm/ext/vector_float3.hpp"
#include "glm/fwd.hpp"
#include <glm/ext/matrix_transform.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

namespace Oglre {

enum class CameraMovement : int {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT,
    UP,
    DOWN
};

class Camera {
public:
    Camera();

    // Camera Attributes.
    glm::vec3 m_cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
    glm::vec3 m_cameraPosition = glm::vec3(200.0f, 200.0f, 400.0f);
    glm::vec3 m_cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
    glm::vec3 m_cameraRight = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 m_worldUp = glm::vec3(0.0f, 1.0f, 0.0f);

    // Camera Options
    // TODO: cameraPitch and cameraFOV both have invariants. Should encapsulate.
    float m_cameraPitch = 0.0f;
    float getSensitivity();
    float m_cameraYaw = -90.0f;
    float m_cameraSpeed = 1000.0f;
    float m_cameraFOV = 90.0f;

    // Camera Flags
    bool m_constrainMovement = false;
    glm::vec2 m_xPosConstraint; // TODO: NOT CURRENTLY UTILIZED.
    glm::vec2 m_nPosConstraint; // TODO: NOT CURRENTLY UTILIZED.
    glm::vec2 m_zPosConstraint; // TODO: NOT CURRENTLY UTILIZED.

    // Get the matrix that defines what the camera "sees".
    glm::mat4 GetCameraViewMatrix();

    bool moveCamera(CameraMovement movement);

    bool rotateCamera(float x_axis_rotate_degrees, float y_axis_rotate_degrees);
    bool zoomCamera(float adjust_FOV);

private:
    // Camera Options
    float m_cameraSensitivity = 0.25f;

    // Update the direction that the camera points, i.e. the camera front vector.
    void updateCameraVectors();
};

}
