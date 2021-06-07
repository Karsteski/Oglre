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
const float defaultCameraFOV = 90.0f;

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
    Camera();
    ~Camera() = default;

    // Get the matrix that defines what the camera "sees".
    static glm::mat4 GetCameraViewMatrix();

    static void KeyboardInput(CameraMovements movement, float deltaTime);
    static void processMouseMovement(float xPositionOffset, float yPositionOffset);
    static void processMouseScroll(float yPositionOffset);

    // Camera Attributes.
    static glm::vec3 cameraPosition;
    static glm::vec3 cameraFront;
    static glm::vec3 cameraUp;
    static glm::vec3 cameraRight;
    static glm::vec3 worldUp;

    // Euler Angles.
    static float cameraPitch;
    static float cameraYaw;

    // Camera Options.
    static float cameraSpeed;
    static float cameraSensitivity;
    static float cameraFOV;

    // Camera Flags.
    static bool constrainMovement;
    static glm::vec2 xPosConstraint;
    static glm::vec2 yPosConstraint;
    static glm::vec2 zPosConstraint;

private:
    // Update the direction that the camera points, i.e. the camera front vector.
    static void updateCameraVectors();
};
}