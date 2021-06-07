#pragma once

// GLEW loads OpenGL function pointers from the system's graphics drivers.
// glew.h MUST be included before gl.h
// clang-format off
#include <GL/glew.h>
#include <GL/gl.h>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
// clang-format on

#include "Camera.h"
namespace Oglre {

// -----------------------
// Application Information
// -----------------------

class Application {
public:
    // --------------------------------
    // Input Handling + Camera Movement
    // --------------------------------

    static int initialWindowWidth;
    static int initialWindowHeight;
    static glm::vec2 lastMousePosition;
    static Camera camera;

    static void processKeyboardInput(GLFWwindow* window); // Manage GLFW's keyboard input
    static void mouseMovementCallback(GLFWwindow* window, double xPosition, double yPosition); // Listen for mouse-movement events
    static void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods); // Listen for mouse button presses.
    static void mouseScrollWheelCallback(GLFWwindow* window, double xPositionOffset, double yPositionOffset); // Listen for mouse scroll wheel.

    static bool IsFirstMouseInput(); // Check if mouse input has been received for the first time.
    static bool MouseButtonPressed(); // Check if right mouse button is being pressed.
    static float GetDeltaTime(); // Get frametime, i.e. the time taken to render a frame.

    // ----------------------
    // OpenGL Error Functions
    // ----------------------

    // Callback function for printing OpenGL debug statements.
    // Note that OpenGL Debug Output must be enabled to utilize glDebugMessageCallback() and consequently this function.
    static void APIENTRY glDebugPrintMessage(GLenum source, GLenum type, unsigned int id, GLenum severity, int length, const char* message, const void* data);

    // ---------------------------
    // Display Related Information
    // ---------------------------

    static void framebufferSizeCallback(GLFWwindow* window, int width, int height);

private:
    static inline bool m_isFirstMouseInput = true;
    static inline bool m_isRightMouseButtonPressed = false;

    Application() {}; // Creating instance of this class is now not possible.
};

}