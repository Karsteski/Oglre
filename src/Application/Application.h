#pragma once

#include "Camera.h"

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include "glm/vec2.hpp"
namespace Oglre {

//
// -----------------------
// Application Information
// -----------------------

class Application {
public:
    static int initialWindowWidth;
    static int initialWindowHeight;
    static glm::vec2 lastMousePosition;

    // ---------------
    // Camera Movement
    // ---------------

    static Camera camera;
    static void processKeyboardInput(GLFWwindow* window); // Manage GLFW's keyboard input
    static void mouseMovementCallback(GLFWwindow* window, double xPosition, double yPosition); // Listen for mouse-movement events
    static void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods); // Listen for mouse button presses.
    static void mouseScrollWheelCallback(GLFWwindow* window, double xPositionOffset, double yPositionOffset); // Listen for mouse scroll wheel.

    static bool IsFirstMouseInput(); // Check if mouse input has been received for the first time.
    static bool MouseButtonPressed(); // Check if right mouse button is being pressed.
    static float GetDeltaTime(); // Get frametime, i.e. the time taken to render a frame.
    static glm::vec2 GetLastMousePosition();

private:
    static inline bool m_isFirstMouseInput = true;
    static inline bool m_isRightMouseButtonPressed = false;

    Application() {}; // Creating instance of this class is now not possible.
};

}