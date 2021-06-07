#include "Application.h"
#include "Camera.h"
#include "glm/ext/vector_float2.hpp"
#include "glm/fwd.hpp"
#include "glm/geometric.hpp"

#include <GLFW/glfw3.h>

// -----------------------
// Application Information
// -----------------------

// Application variables
int Oglre::Application::initialWindowWidth = 800;
int Oglre::Application::initialWindowHeight = 600;

glm::vec2 Oglre::Application::lastMousePosition = glm::vec2(initialWindowWidth / 2.0f, initialWindowHeight / 2.0f);

bool Oglre::Application::IsFirstMouseInput()
{
    if (m_isFirstMouseInput) {
        m_isFirstMouseInput = false;
        return true;
    } else {
        return false;
    }
}

bool Oglre::Application::MouseButtonPressed()
{
    if (m_isRightMouseButtonPressed) {
        m_isRightMouseButtonPressed = false;
        return true;
    } else {
        return false;
    }
}

float Oglre::Application::GetDeltaTime()
{
    static float deltaTime = 0.0f;
    static float currentTime = 0.0f;
    static float lastTime = 0.0f;

    currentTime = glfwGetTime();
    deltaTime = currentTime - lastTime;
    lastTime = currentTime;

    return deltaTime;
}

// --------------------------------
// Input Handling + Camera Movement
// --------------------------------

void Oglre::Application::processKeyboardInput(GLFWwindow* window)
{
    const float deltaTime = Application::GetDeltaTime();

    // The resulting right vectors are normalized as the camera speed would otherwise be based on the camera's orientation.
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        camera.KeyboardInput(Oglre::CameraMovements::FORWARD, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        camera.KeyboardInput(Oglre::CameraMovements::BACKWARD, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        camera.KeyboardInput(Oglre::CameraMovements::LEFT, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        camera.KeyboardInput(Oglre::CameraMovements::RIGHT, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {
        camera.KeyboardInput(Oglre::CameraMovements::UP, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
        camera.KeyboardInput(Oglre::CameraMovements::DOWN, deltaTime);
    }
}

void Oglre::Application::mouseMovementCallback(GLFWwindow* window, double xPosition, double yPosition)
{
    if (Application::IsFirstMouseInput()) {
        Application::lastMousePosition.x = xPosition;
        Application::lastMousePosition.y = yPosition;
    }

    float xPositionOffset = xPosition - Application::lastMousePosition.x;
    float yPositionOffset = yPosition - Application::lastMousePosition.y;

    Application::lastMousePosition.x = xPosition;
    Application::lastMousePosition.y = yPosition;

    xPositionOffset *= camera.cameraSensitivity;
    yPositionOffset *= camera.cameraSensitivity;

    camera.cameraYaw += xPositionOffset;
    camera.cameraPitch += yPositionOffset;

    // Constrain pitch because at 90 degrees the LookAt function flips the camera direction.
    if (camera.cameraPitch > 89.0f) {
        camera.cameraPitch = 89.0f;
    }
    if (camera.cameraPitch < -89.0f) {
        camera.cameraPitch = -89.0f;
    }

    // Create camera direction vector using Euler angles.
    glm::vec3 cameraDirection;

    // Must convert to radians first.
    // Note that xz sides are influenced by cos(pitch) and must therefore be included in their calculations.
    cameraDirection.x = std::cos(glm::radians(camera.cameraYaw)) * std::cos(glm::radians(camera.cameraPitch));
    cameraDirection.y = sin(glm::radians(camera.cameraPitch));
    cameraDirection.z = std::sin(glm::radians(camera.cameraYaw)) * std::cos(glm::radians(camera.cameraPitch));

    // Set camera direction vector if mouse is pressed.
    if (m_isRightMouseButtonPressed) {
        camera.cameraFront = glm::normalize(cameraDirection);
    }
}

void Oglre::Application::mouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
    if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS) {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        glfwSetInputMode(window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);

        // Enable camera movement with mouse.
        m_isRightMouseButtonPressed = true;
    } else {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        glfwSetInputMode(window, GLFW_RAW_MOUSE_MOTION, GLFW_FALSE);

        // Disable camera movement with mouse.
        m_isRightMouseButtonPressed = false;
    }
}

void Oglre::Application::mouseScrollWheelCallback(GLFWwindow* window, double xPositionOffset, double yPositionOffset)
{
    camera.cameraFOV -= static_cast<float>(yPositionOffset);

    // Constrain zoom/FOV values
    if (camera.cameraFOV < 1.0f) {
        camera.cameraFOV = 1.0f;
    }
    if (camera.cameraFOV > 90.0f) {
        camera.cameraFOV = 90.0f;
    }
}
