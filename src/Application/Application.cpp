#include "Application.h"
#include "Camera.h"
#include "IndexBuffer.h"
#include "Renderer.h"
#include "Shader.h"
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"

// Maths Library
#include <GLFW/glfw3.h>
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
    // TODO: STILL A LOT OF CLEANING UP TO DO AS I REFACTOR

    GLFWwindow* window = Oglre::Application::GetWindow();

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

    // Instantiate Renderer.
    Renderer renderer;

    // Instantiate Camera.
    Oglre::Camera camera;

    // Model View Projection matrices
    // First matrix is an identity matrix, second matrix is the translation matrix that moves the view.
    glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0)); // Move "object" 100 units up and right.
    glm::mat4 projection;
    glm::mat4 mvpMatrix;

    // DearImGUI things
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    (void)io;

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();

    // Setup Platform/Renderer backends
    std::string glsl_version = "#version 330";

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version.c_str());

    // Render and event loop.
    while (!glfwWindowShouldClose(window)) {
        updateDeltaTime();
        // DearImGUI things
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // Flags
        static int f_Projection = 0;

        // Basic Camera controls through DearImGUI
        {
            ImGui::Begin("Camera Controls");

            ImGui::SliderFloat3("Camera Position", &camera.m_cameraPosition[0], 0.0f, 1000.0f);
            ImGui::SliderFloat3("Camera View", &camera.m_cameraFront[0], 0.0f, 1000.0f);

            if (ImGui::Button("Reset Camera")) {
                camera.m_cameraPosition = glm::vec3(200.0f, 200.0f, 200.0f);
                camera.m_cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
            }

            static bool wireframeMode = false;
            ImGui::Checkbox("Enable Wireframe Mode", &wireframeMode);
            if (wireframeMode)
                renderer.EnableWireFrameMode(true);
            else
                renderer.EnableWireFrameMode(false);

            // Set projection matrix (orthographic or perspective)
            ImGui::RadioButton("Perspective Projection", &f_Projection, 0);
            ImGui::SameLine();
            ImGui::RadioButton("Orthographic Projection", &f_Projection, 1);

            ImGui::End();
        }

        // Match viewport size with current window size
        static int currentWindowWidth = 0;
        static int currentWindowHeight = 0;
        glfwGetWindowSize(window, &currentWindowWidth, &currentWindowHeight);
        glfwSetFramebufferSizeCallback(window, Oglre::Application::FramebufferSizeCallback);

        // Process keyboard commands.
        Oglre::Application::ProcessKeyboardInput(window, camera);

        // Process mouse input and movement.
        
        // TODO: Need to move these to free functions probably
        // glfwSetMouseButtonCallback(window, Oglre::Application::MouseButtonCallback);
        // glfwSetCursorPosCallback(window, Oglre::Application::MouseMovementCallback);
        // glfwSetScrollCallback(window, Oglre::Application::MouseScrollWheelCallback);

        // Projection matrix for use in the Vertex Shader.
        if (f_Projection == 0) {
            projection = glm::perspective(glm::radians(camera.m_cameraFOV), (float)currentWindowWidth / currentWindowHeight, 0.1f, 10000.0f);
        } else if (f_Projection == 1) {
            // TODO: Works, but I need the object to be in the same position when switching between perspectives (Possible..?)
            static float min = -pow(10, glm::radians(camera.m_cameraFOV));
            static float max = pow(10, glm::radians(camera.m_cameraFOV));

            projection = glm::ortho(min, max, min, max, -10000.0f, 10000.0f);
        }

        // Set MVP matrix once projection matrix has been updated.
        // Note that the calculation is actually Projection * View * Model as OpenGL uses column major ordering by default.
        // This affects how the MVP Matrix must be created.
        mvpMatrix = projection * camera.GetCameraViewMatrix() * model;

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
}

void Oglre::Application::Exit()
{
    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(m_window);
    glfwTerminate();

    exit(EXIT_SUCCESS);
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

float Oglre::Application::updateDeltaTime()
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

void Oglre::Application::ProcessKeyboardInput(GLFWwindow* window, Camera& camera)
{
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        camera.moveCamera(CameraMovement::FORWARD);
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        camera.moveCamera(CameraMovement::BACKWARD);
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {

        camera.moveCamera(CameraMovement::LEFT);
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {

        camera.moveCamera(CameraMovement::RIGHT);
    }
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {

        camera.moveCamera(CameraMovement::UP);
    }
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
        camera.moveCamera(CameraMovement::DOWN);
    }
}

/* void Oglre::Application::MouseMovementCallback(GLFWwindow* window, double xPosition, double yPosition)
{
    if (Application::IsFirstMouseInput()) {
        Application::lastMousePosition.x = xPosition;
        Application::lastMousePosition.y = yPosition;
    }

    float xPositionOffset = xPosition - Application::lastMousePosition.x;
    float yPositionOffset = yPosition - Application::lastMousePosition.y;

    Application::lastMousePosition.x = xPosition;
    Application::lastMousePosition.y = yPosition;

    xPositionOffset *= m_camera.cameraSensitivity;
    yPositionOffset *= m_camera.cameraSensitivity;

    m_camera.cameraYaw += xPositionOffset;
    m_camera.cameraPitch += yPositionOffset;

    // Constrain pitch because at 90 degrees the LookAt function flips the camera direction.
    if (m_camera.cameraPitch > 89.0f) {
        m_camera.cameraPitch = 89.0f;
    }
    if (m_camera.cameraPitch < -89.0f) {
        m_camera.cameraPitch = -89.0f;
    }

    // Create camera direction vector using Euler angles.
    glm::vec3 cameraDirection;

    // Must convert to radians first.
    // Note that xz sides are influenced by cos(pitch) and must therefore be included in their calculations.
    cameraDirection.x = std::cos(glm::radians(m_camera.cameraYaw)) * std::cos(glm::radians(m_camera.cameraPitch));
    cameraDirection.y = sin(glm::radians(m_camera.cameraPitch));
    cameraDirection.z = std::sin(glm::radians(m_camera.cameraYaw)) * std::cos(glm::radians(m_camera.cameraPitch));

    // Set camera direction vector if mouse is pressed.
    if (m_isRightMouseButtonPressed) {
        m_camera.cameraFront = glm::normalize(cameraDirection);
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
    m_camera.cameraFOV -= static_cast<float>(yPositionOffset);

    // Constrain zoom/FOV values
    if (m_camera.cameraFOV < 1.0f) {
        m_camera.cameraFOV = 1.0f;
    }
    if (m_camera.cameraFOV > 90.0f) {
        m_camera.cameraFOV = 90.0f;
    }
}
 */

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
