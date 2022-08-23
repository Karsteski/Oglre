#pragma once

#include "glm/ext/vector_float3.hpp"
#include "glm/fwd.hpp"
#include <glm/ext/matrix_transform.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

namespace Oglre {

// TODO: Should be defined in .cpp
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

    // Camera Attributes.
    static inline glm::vec3 cameraPosition = glm::vec3(200.0f, 200.0f, 400.0f);
    static inline glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
    static inline glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
    static inline glm::vec3 cameraRight = glm::vec3(0.0f, 0.0f, 0.0f);
    static inline glm::vec3 worldUp = glm::vec3(0.0f, 1.0f, 0.0f);

    // Camera Options
    static inline float cameraPitch = 0.0f;
    static inline float cameraYaw = -90.0f;
    static inline float cameraSpeed = 1000.0f;
    static inline float cameraSensitivity = 0.25f;
    static inline float cameraFOV = 90.0f;

    // Camera Flags
    static inline bool constrainMovement = false;
    static inline glm::vec2 xPosConstraint; // TODO: NOT CURRENTLY UTILIZED.
    static inline glm::vec2 yPosConstraint; // TODO: NOT CURRENTLY UTILIZED.
    static inline glm::vec2 zPosConstraint; // TODO: NOT CURRENTLY UTILIZED.

    // Get the matrix that defines what the camera "sees".
    static glm::mat4 GetCameraViewMatrix();

    // TODO: Camera should NOT be processing keyboard input. It should only take a CameraMovements
    static void KeyboardInput(CameraMovements movement, float deltaTime);
    static void processMouseMovement(float xPositionOffset, float yPositionOffset);
    static void processMouseScroll(float yPositionOffset);

private:
    // Update the direction that the camera points, i.e. the camera front vector.
    static void updateCameraVectors();
};
}
