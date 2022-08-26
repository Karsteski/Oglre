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

    float getFOV();
    float getSensitivity();
    bool getConstrainment();

    glm::vec3 getCameraFront();
    glm::vec3 getPosition();

    // Get the matrix that defines what the camera "sees".
    glm::mat4 GetCameraViewMatrix();

    void setCameraFront(glm::vec3 cameraFront);
    void setCameraPosition(glm::vec3 cameraPosition);

    void constrainMovement(bool constrain);
    bool moveCamera(CameraMovement movement);
    bool rotateCamera(float x_axis_rotate_degrees, float y_axis_rotate_degrees);
    bool zoomCamera(float adjust_FOV);

private:
    // Camera Options
    float m_cameraPitch;
    float m_cameraYaw;
    float m_cameraSpeed;
    float m_cameraFOV;
    float m_cameraSensitivity;

    // Camera Attributes.
    glm::vec3 m_cameraFront;
    glm::vec3 m_cameraPosition;
    glm::vec3 m_cameraUp;
    glm::vec3 m_cameraRight;
    glm::vec3 m_worldUp;

    // Camera Flags
    bool m_constrainMovement = false;

    // Update the direction that the camera points, i.e. the camera front vector.
    void updateCameraVectors();
};

}
