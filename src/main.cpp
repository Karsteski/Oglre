// GLEW loads OpenGL function pointers from the system's graphics drivers.
// glew.h MUST be included before gl.h
// clang-format off
#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "glm/geometric.hpp"
#include <GL/glew.h>
#include <GL/gl.h>

// Disables inclusion of the dev-environ header.
// Allows GLFW + extension loader headers to be included in any order.
// GLFW including OpenGL headers causes function definition ambiguity.
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
// clang-format on

// Maths Library
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

// DearImGUI
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include <array>
#include <cmath>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "Application.h"
#include "Camera.h"
#include "IndexBuffer.h"
#include "Renderer.h"
#include "Shader.h"
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"

// Globals
const int initialWindowWidth = 800;
const int initialWindowHeight = 600;
std::string shaderPath = "../resources/shaders/Basic.glsl";

// Callback function for printing OpenGL debug statements.
// Note that OpenGL Debug Output must be enabled to utilize glDebugMessageCallback() and consequently this function.
void APIENTRY glDebugPrintMessage(GLenum source, GLenum type, unsigned int id, GLenum severity, int length, const char* message, const void* data)
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

// Move to Oglre namespace w/ other abstracted GLFW setup (Application.cpp)
void framebufferSizeCallback(GLFWwindow* window, int width, int height)
{
    // Ensure viewport matches new window dimensions.
    glViewport(0, 0, width, height);

    // Should re-render scene after calling glfwSetFramebufferSizeCallback() as current frame
    // would have been drawn for the old viewport size.
}
/*
// These variables will be part of the camera class
// CameraFront is the direction vector pointing in the opposite direction of the where the camera is pointed.
glm::vec3 cameraPosition = glm::vec3(200.0f, 200.0f, 400.0f);
 glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
 glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);


// Keep track of frame rate
// float deltaTime = 0.0f;
// float lastFrameTime = 0.0f;

// Globals for mouse movement trigonometry calculation.
// TODO: This all needs to be in camera class.
static bool isRightMouseButtonPressed = false;
static bool isFirstMouseInput = true;

float pitch = 0.0f;
float yaw = -90.0f; // Set to this value to ensure camera points into the screen i.e. negative z-axis

// For mouse movement
// Store last mouse positions in the application, initialized to the center of the screen.
float lastXPosition = initialWindowWidth / 2.0f;
float lastYPosition = initialWindowHeight / 2.0f;

// Definites and allows changes to the viewport FOV (i.e. zooming in/out)
float mouseZoom = 90.0f;

// Manage GLFW's keyboard input
void processKeyboardInput(GLFWwindow* window)
{
    const float cameraSpeed = 1000.0f * Oglre::Application::GetDeltaTime();

    // The resulting right vectors are normalized as the camera speed would otherwise be based on the camera's orientation.
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        cameraPosition += cameraSpeed * cameraFront;
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        cameraPosition -= cameraSpeed * glm::normalize(glm::cross(cameraFront, cameraUp));
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        cameraPosition -= cameraSpeed * cameraFront;
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        cameraPosition += cameraSpeed * glm::normalize(glm::cross(cameraFront, cameraUp));
    }
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {
        cameraPosition += cameraSpeed * cameraUp;
    }
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
        cameraPosition -= cameraSpeed * cameraUp;
    }
}

// Listen for mouse-movement events
void mouseMovementCallback(GLFWwindow* window, double xPosition, double yPosition)
{
    // Check to see if this is the first time receiving mouse input.
    if (Oglre::Application::IsFirstMouseInput()) {
        lastXPosition = xPosition;
        lastYPosition = yPosition;
        isFirstMouseInput = false;
    }

    float xPositionOffset = xPosition - lastXPosition;
    float yPositionOffset = yPosition - lastYPosition;

    lastXPosition = xPosition;
    lastYPosition = yPosition;

    const float sensitivity = 1.0f;
    xPositionOffset *= sensitivity;
    yPositionOffset *= sensitivity;

    yaw += xPositionOffset;
    pitch += yPositionOffset;

    // Constrain pitch because at 90 degrees the LookAt function flips the camera direction.
    if (pitch > 89.0f) {
        pitch = 89.0f;
    }
    if (pitch < -89.0f) {
        pitch = -89.0f;
    }

    // Create camera direction vector using Euler angles.
    glm::vec3 cameraDirection;

    // Must convert to radians first.
    // Note that xz sides are influenced by cos(pitch) and must therefore be included in their calculations.
    cameraDirection.x = std::cos(glm::radians(yaw)) * std::cos(glm::radians(pitch));
    cameraDirection.y = sin(glm::radians(pitch));
    cameraDirection.z = std::sin(glm::radians(yaw)) * std::cos(glm::radians(pitch));

    // Calculate direction vector if mouse is pressed.
    if (isRightMouseButtonPressed) {
        cameraFront = glm::normalize(cameraDirection);
    }
}

// Listen for mouse button presses.
void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
    if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS) {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        glfwSetInputMode(window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);

        // Enable camera movement with mouse.
        isRightMouseButtonPressed = true;
    } else {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        glfwSetInputMode(window, GLFW_RAW_MOUSE_MOTION, GLFW_FALSE);

        // Disable camera movement with mouse.
        isRightMouseButtonPressed = false;
    }
}

// Listen for mouse scroll wheel.
void mouseScrollWheelCallback(GLFWwindow* window, double xPositionOffset, double yPositionOffset)
{
    mouseZoom -= static_cast<float>(yPositionOffset);

    // Constrain zoom values
    //if (mouseZoom < 1.    cameraPosition = positionVector;
    //worldUp = upVector;

    //cameraYaw = yaw;
    //cameraPitch = pitch;
    if (mouseZoom > 90.0f) {
        mouseZoom = 90.0f;
    }
}
*/
int main()
{
    // C++ version verification.
    std::cout << "C++ version = ";
    if (__cplusplus == 202002L)
        std::cout << "C++20\n";
    else if (__cplusplus == 201703L)
        std::cout << "C++17\n";
    else if (__cplusplus == 201402L)
        std::cout << "C++14\n";
    else if (__cplusplus == 201103L)
        std::cout << "C++11\n";
    else if (__cplusplus == 199711L)
        std::cout << "C++98\n";
    else
        std::cout << "pre-standard C++\n";

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

    GLFWwindow* window = glfwCreateWindow(initialWindowWidth, initialWindowHeight, "Oglre", NULL, NULL);
    if (!window) {
        std::cout << "GLFW Window creation failed\n"
                  << "Exiting...\n";

        exit(EXIT_FAILURE);
    }

    glfwMakeContextCurrent(window);

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

        glDebugMessageCallback(glDebugPrintMessage, nullptr);

        std::cout << ("OpenGL Debug Mode\n");
    } else {
        std::cout << "Debug for OpenGL not supported by the system!\n";
    }

    // Printing OpenGL version for convenience.
    std::cout << "OpenGL Version + System GPU Drivers: " << glGetString(GL_VERSION) << std::endl;

    // clang-format off
    // Cube positions.
    std::vector<float> vertices = {
        // Positions                // Colours
        100.0f, 100.0f, 100.0f,     1.0f, 0.0f, 0.0f, // 0
        -100.0f, 100.0f, 100.0f,    0.0f, 1.0f, 0.0f, // 1
        -100.0f, 100.0f, -100.0f,   0.0f, 0.0f, 1.0f, // 2
        100.0f, 100.0f, -100.0f,    1.0f, 1.0f, 1.0f, // 3
        100.0f, -100.0f, 100.0f,    1.0f, 0.0f, 0.0f, // 4
        -100.0f, -100.0f, 100.0f,   0.0f, 1.0f, 0.0f, // 5
        -100.0f, -100.0f, -100.0f,  0.0f, 0.0f, 1.0f, // 6
        100.0f, -100.0f, -100.0f,   1.0f, 1.0f, 1.0f  // 7
    };

    // Index Buffer.
    // References the vector of positions necessary to draw 12 triangles making up a cube.
    std::vector<unsigned int> indices {
        0, 1, 3, // top 1
        3, 1, 2, // top 2
        2, 6, 7, // front 1
        7, 3, 2, // front 2
        7, 6, 5, // bottom 1
        5, 4, 7, // bottom 2
        5, 1, 4, // back 1
        4, 1, 0, // back 2
        4, 3, 7, // right 1
        3, 4, 0, // right 2
        5, 6, 2, // left 1
        5, 1, 2  // left 2
    };
    // clang-format on

    // Create Vertex Array.
    VertexArray va;

    // Generate Vertex Buffer Object.
    const int nPoints = 3 * 2 * 8;
    VertexBuffer vbo(vertices, nPoints * sizeof(float));

    // Create Vertex Buffer Layout.
    VertexBufferLayout layout;
    const int nFloatsPerVertexAttribute = 3;

    // One Push for each vertex attribute.
    layout.Push<float>(nFloatsPerVertexAttribute);
    layout.Push<float>(nFloatsPerVertexAttribute);
    va.AddBuffer(vbo, layout);

    // Generate Index Buffer.
    const int numberOfIndices = 3 * 12;
    IndexBuffer ibo(indices, numberOfIndices);

    // Deal with vertex and fragment shader.
    Shader shader(shaderPath);
    shader.Bind();
    // shader.SetUniform("u_Colour", 1.0f, 1.0f, 1.0f, 1.0f);

    // Instantiate Renderer.
    Renderer renderer;

    // Instantiate Camera.
    //Oglre::Camera camera(glm::vec3(200.0f, 200.0f, 400.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    Oglre::Camera camera;
    // Model View Projection matrices

    // First matrix is an identity matrix, second matrix is the translation matrix that moves the view.
    glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0)); // Move "object" 100 units up and right.
    glm::mat4 perspectiveProjection;
    glm::mat4 mvpMatrix;

    // DearImGUI things
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    (void)io;

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsClassic();

    // Setup Platform/Renderer backends
    std::string glsl_version = "#version 330";

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version.c_str());

    // Render and event loop.
    while (!glfwWindowShouldClose(window)) {

        // Calculate time per frame
        // float currentFrameTime = glfwGetTime();
        // deltaTime = currentFrameTime - lastFrameTime;
        // lastFrameTime = currentFrameTime;

        // DearImGUI things
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        //bool show_demo_window = true;
        //ImGui::ShowDemoWindow(&show_demo_window);

        // The direction is given by the sum of the cameraPosition + cameraFront (direction vector) to ensure camera keeps looking
        // in the target direction.
        
        //       glm::mat4 cameraView = glm::lookAt(cameraPosition, cameraPosition + cameraFront, cameraUp);

        // Basic Camera controls through DearImGUI
        {
            ImGui::Begin("Camera Controls");

            ImGui::SliderFloat3("Camera Position", &camera.cameraPosition[0], 0.0f, 1000.0f);
            ImGui::SliderFloat3("Camera View", &camera.cameraFront[0], 0.0f, 1000.0f);

            if (ImGui::Button("Reset Camera")) {
                camera.cameraPosition = glm::vec3(200.0f, 200.0f, 200.0f);
                camera.cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
            }

            static bool wireframeMode = false;
            ImGui::Checkbox("Enable Wireframe Mode", &wireframeMode);
            if (wireframeMode)
                renderer.EnableWireFrameMode(true);
            else
                renderer.EnableWireFrameMode(false);

            ImGui::End();
        }

        // For updating viewport size.
        int currentWindowWidth = 0;
        int currentWindowHeight = 0;
        float aspectRatio = 0;

        // Match viewport size with current window size
        glfwGetWindowSize(window, &currentWindowWidth, &currentWindowHeight);
        glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);
        aspectRatio = static_cast<float>(currentWindowWidth) / static_cast<float>(currentWindowHeight);

        // Process keyboard commands.
        Oglre::Application::processKeyboardInput(window);
        //processKeyboardInput(window);

        // Process mouse input and movement.
        /*
        glfwSetMouseButtonCallback(window, mouseButtonCallback);
        glfwSetCursorPosCallback(window, mouseMovementCallback);
        glfwSetScrollCallback(window, mouseScrollWheelCallback);
        */
        glfwSetMouseButtonCallback(window, Oglre::Application::mouseButtonCallback);
        glfwSetCursorPosCallback(window, Oglre::Application::mouseMovementCallback);
        glfwSetScrollCallback(window, Oglre::Application::mouseScrollWheelCallback);

        // Orthographic projection matrix for use in the Vertex Shader.
        //orthoProjection = glm::ortho(0.0f, (float)currentWindowWidth, 0.0f, (float)currentWindowHeight, -1.0f, 1.0f);
        perspectiveProjection = glm::perspective(glm::radians(camera.cameraFOV), (float)currentWindowWidth / currentWindowHeight, 0.1f, 1000.0f);
        // Set MVP matrix once projection matrix has been updated.
        // Note that the calculation is actually Projection * View * Model as OpenGL uses column major ordering by default.
        // This affects how the MVP Matrix must be created.
        mvpMatrix = perspectiveProjection * camera.GetCameraViewMatrix() * model;
        // Now shader can be set.
        shader.SetUniformMat4f("u_MVP", mvpMatrix);

        // Render from this point on.
        renderer.Clear();
        renderer.Draw(va, ibo, shader);

        // DearImGUI things
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        // Swaps the front and back buffers of the specified window.
        // The front buffer is the current buffer shown on screen, whilst the back is the data to be drawn to.
        glfwSwapBuffers(window);

        // Poll and process events.
        glfwPollEvents();
    }

    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();

    exit(EXIT_SUCCESS);
}