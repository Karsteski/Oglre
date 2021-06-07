#include "Application.h"
#include "Camera.h"

#include <iostream>
#include <string>

// ---------------------
// Application Execution
// ---------------------

void Oglre::Application::Initialize()
{
    // GLFW Setup
    if (!glfwInit()) {
        std::cout << "GLFW Initialization failed!\n"
                  << "Exiting...\n";

        exit(EXIT_FAILURE);
    }

    // OpenGL version and mode setup.
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    // GLFW Window settings
    glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);

    // For OpenGL debugging.
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);

    Application::m_window = glfwCreateWindow(initialWindowWidth, initialWindowHeight, "Oglre", NULL, NULL);
    if (!m_window) {
        std::cout << "GLFW Window creation failed\n"
                  << "Exiting...\n";

        exit(EXIT_FAILURE);
    }

    glfwMakeContextCurrent(m_window);

    // A valid OpenGL context must be created before initializing GLEW.
    // Initialize OpenGL loader (GLEW in this project).
    bool error = glewInit();
    if (error != GLEW_OK) {
        std::cout << "Error: Failed to initialize OpenGL function pointer loader!\n";
    }

    // Enable debugging layer of OpenGL
    int glFlags = 0;
    glGetIntegerv(GL_CONTEXT_FLAGS, &glFlags);
    if (glFlags & GL_CONTEXT_FLAG_DEBUG_BIT) {
        glEnable(GL_DEBUG_OUTPUT);
        glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);

        glDebugMessageCallback(GLDebugPrintMessage, nullptr);

        std::cout << ("OpenGL Debug Mode\n");
    } else {
        std::cout << "Debug for OpenGL not supported by the system!\n";
    }

    // Printing OpenGL version for convenience.
    std::cout << "OpenGL Version + System GPU Drivers: " << glGetString(GL_VERSION) << std::endl;
}

void Oglre::Application::Run()
{
}

void Oglre::Application::Exit()
{
}

GLFWwindow* Oglre::Application::GetWindow()
{
    return m_window;
}

// -----------------------
// Application Information
// -----------------------

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

void Oglre::Application::ProcessKeyboardInput(GLFWwindow* window)
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

void Oglre::Application::MouseMovementCallback(GLFWwindow* window, double xPosition, double yPosition)
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

void Oglre::Application::MouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
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

void Oglre::Application::MouseScrollWheelCallback(GLFWwindow* window, double xPositionOffset, double yPositionOffset)
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

// ----------------------
// OpenGL Error Functions
// ----------------------
void APIENTRY Oglre::Application::GLDebugPrintMessage(GLenum source, GLenum type, unsigned int id, GLenum severity, int length, const char* message, const void* data)
{
    /*

    To enable the debugging layer of OpenGL:

    glEnable(GL_DEBUG_OUTPUT); - This is a faster version but there are no debugger breakpoints.
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS); - Callback is synchronized w/ errors, and a breakpoint can be placed on the callback to get a stacktrace for the GL error. 
    
    // Followed by the call:
    glDebugMessageCallback(glDebugPrintMessage, nullptr);
    */

    std::string sourceMessage = "";
    std::string typeMessage = "";
    std::string severityMessage = "";

    switch (source) {
    case GL_DEBUG_SOURCE_API: {
        sourceMessage = "API";
        break;
    }
    case GL_DEBUG_SOURCE_WINDOW_SYSTEM: {
        sourceMessage = "WINDOW SYSTEM";
        break;
    }
    case GL_DEBUG_SOURCE_SHADER_COMPILER: {
        sourceMessage = "SHADER COMPILER";
        break;
    }
    case GL_DEBUG_SOURCE_THIRD_PARTY: {
        sourceMessage = "THIRD PARTY";
        break;
    }
    case GL_DEBUG_SOURCE_APPLICATION: {
        sourceMessage = "APPLICATION";
        break;
    }
    case GL_DEBUG_SOURCE_OTHER: {
        sourceMessage = "UNKNOWN";
        break;
    }
    default: {
        sourceMessage = "UNKNOWN";
        break;
    }
    }

    switch (type) {
    case GL_DEBUG_TYPE_ERROR: {
        typeMessage = "ERROR";
        break;
    }
    case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: {
        typeMessage = "DEPRECATED BEHAVIOUR";
        break;
    }
    case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR: {
        typeMessage = "UNDEFINED BEHAVIOUR";
        break;
    }
    case GL_DEBUG_TYPE_PORTABILITY: {
        typeMessage = "PORTABILITY";
        break;
    }
    case GL_DEBUG_TYPE_PERFORMANCE: {
        typeMessage = "PERFORMANCE";
        break;
    }
    case GL_DEBUG_TYPE_OTHER: {
        typeMessage = "OTHER";
        break;
    }
    case GL_DEBUG_TYPE_MARKER: {
        typeMessage = "MARKER";
        break;
    }
    default: {
        typeMessage = "UNKNOWN";
        break;
    }
    }

    switch (severity) {
    case GL_DEBUG_SEVERITY_HIGH: {
        severityMessage = "HIGH";
        break;
    }
    case GL_DEBUG_SEVERITY_MEDIUM: {
        severityMessage = "MEDIUM";
        break;
    }
    case GL_DEBUG_SEVERITY_LOW: {
        severityMessage = "LOW";
        break;
    }
    case GL_DEBUG_SEVERITY_NOTIFICATION: {
        severityMessage = "NOTIFICATION";
        break;
    }
    default: {
        severityMessage = "UNKNOWN";
        break;
    }
    }

    std::cout << id << ": " << typeMessage << " of " << severityMessage << ", raised from "
              << sourceMessage << ": " << message << std::endl;
}

// ---------------------------
// Display Related Information
// ---------------------------

void Oglre::Application::FramebufferSizeCallback(GLFWwindow* window, int width, int height)
{
    // Ensure viewport matches new window dimensions.
    glViewport(0, 0, width, height);

    // Should re-render scene after calling glfwSetFramebufferSizeCallback() as current frame
    // would have been drawn for the old viewport size.
}