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

#include <string>

namespace Oglre {

// -----------------------
// Application Information
// -----------------------

class Application {
public:
    // ---------------------
    // Application Execution
    // ---------------------

    static void Initialize();
    static void Run();
    static void Exit();

    static GLFWwindow* GetWindow();

    static inline std::string shaderPath = "../resources/shaders/Basic.glsl"; // TODO: Function that returns all shader paths.

    // --------------------------------
    // Input Handling + Camera Movement
    // --------------------------------

    static constexpr int initialWindowWidth = 800;
    static constexpr int initialWindowHeight = 600;
    static inline glm::vec2 lastMousePosition = glm::vec2(initialWindowWidth / 2.0f, initialWindowHeight / 2.0f);

    static inline Camera m_camera = Camera();

    static void processKeyboardInput(GLFWwindow* window, Camera& camera); // Manage GLFW's keyboard input
    static void mouseMovementCallback(GLFWwindow* window, double xPosition, double yPosition); // Listen for mouse-movement events
    static void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods); // Listen for mouse button presses.
    static void mouseScrollWheelCallback(GLFWwindow* window, double xPositionOffset, double yPositionOffset); // Listen for mouse scroll wheel.

    static bool isFirstMouseInput(); // Check if mouse input has been received for the first time.
    static bool mouseButtonPressed(); // Check if right mouse button is being pressed.
    static float updateDeltaTime(); // Get frametime, i.e. the time taken to render a frame.

    // ----------------------
    // OpenGL Error Functions
    // ----------------------

    // Callback function for printing OpenGL debug statements.
    // Note that OpenGL Debug Output must be enabled to utilize glDebugMessageCallback() and consequently this function.
    static void APIENTRY GLDebugPrintMessage(GLenum source, GLenum type, unsigned int id, GLenum severity, int length, const char* message, const void* data);

    // ---------------------------
    // Display Related Information
    // ---------------------------

    static void FramebufferSizeCallback(GLFWwindow* window, int width, int height);

private:
    static inline GLFWwindow* m_window = nullptr;

    static inline bool m_isFirstMouseInput = true;
    static inline bool m_isRightMouseButtonPressed = false;

    Application() {}; // Creating instance of this class is now not possible.
};

}
