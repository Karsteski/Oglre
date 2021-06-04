#pragma once

#include "glm/ext/vector_float3.hpp"
#include "glm/fwd.hpp"
#include <glm/ext/matrix_transform.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

namespace Oglre {

const float defaultCameraPitch = 0.0f;
const float defaultCameraYaw = -90.0f;
const float defaultCameraSpeed = 1000.0f;
const float defaultCameraSensitivity = 1.0f;
const float defaultCameraFOV = glm::radians(90.0f);

// Possible options for the movement of the camera.
enum class CameraMovements {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT,
    UP,
    DOWN
};

class Camera {
public:
    Camera(glm::vec3 positionVector = glm::vec3(200.0f, 200.0f, 400.0f), glm::vec3 upVector = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = defaultCameraYaw, float pitch = defaultCameraPitch);
    ~Camera() = default;

    // Get the matrix that defines what the camera "sees".
    glm::mat4 GetCameraViewMatrix();

    void processKeyboardInput(CameraMovements movement, float deltaTime);
    void processMouseMovement(float xPositionOffset, float yPositionOffset);
    void processMouseScroll(float yPositionOffset);

    // Camera Attributes.
    glm::vec3 cameraPosition;
    glm::vec3 cameraFront;
    glm::vec3 cameraUp;
    glm::vec3 cameraRight;
    glm::vec3 worldUp;

    // Euler Angles.
    float cameraPitch;
    float cameraYaw;

    // Camera Options.
    float cameraSpeed;
    float cameraSensitivity;
    float cameraFOV;

    // Camera Flags.
    bool constrainPitch;

private:
    // Update the direction that the camera points, i.e. the camera front vector.
    void updateCameraVectors();
};
}