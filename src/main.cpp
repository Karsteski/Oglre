// GLEW loads OpenGL function pointers from the system's graphics drivers.
// glew.h MUST be included before gl.h
// clang-format off
#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/matrix_transform.hpp"
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
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

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
        100.0f, 100.0f, -100.0f,    1.0f, 0.0f, 0.0f, // 3
        100.0f, -100.0f, 100.0f,    0.0f, 1.0f, 0.0f, // 4
        -100.0f, -100.0f, 100.0f,   0.0f, 0.0f, 1.0f, // 5
        -100.0f, -100.0f, -100.0f,  1.0f, 0.0f, 0.0f, // 6
        100.0f, -100.0f, -100.0f,   0.0f, 1.0f, 0.0f  // 7
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
    const int nPoints = 6 * 8;
    VertexBuffer vbo(vertices, nPoints * sizeof(float));

    // Create Vertex Buffer Layout.
    VertexBufferLayout layout;
    const int nFloatsPerVertexAttribute = 3;
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

    // Model View Projection matrices

    // First matrix is an identity matrix, second matrix is the translation matrix that moves the view.
    glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0)); // Move "object" 100 units up and right.
    //glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(100.0f, 200.0f, 200.0f)); // Move "camera" 100 units right.
    // glm::mat4 orthoProjection;
    glm::mat4 perspectiveProjection;
    glm::mat4 mvpMatrix;

    /*
    glm::mat4 view = glm::lookAt(
        glm::vec3(200.0f, 200.0f, 200.0f),
        glm::vec3(150.0f, 150.0f, 100.0f),
        glm::vec3(0.0f, 1.0f, 0.0f));
    */

    /*
    float camLocX = 200.0f;
    float camLocY = 200.0f;
    float camLocZ = 200.0f;

    float camViewX = 150.0f;
    float camViewY = 150.0f;
    float camViewZ = 100.0f;
    */

    glm::vec3 cameraPositionVector = glm::vec3(200.0f, 200.0f, 200.0f);
    glm::vec3 cameraViewVector = glm::vec3(150.0f, 150.0f, 100.0f);

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

        // DearImGUI things
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        //bool show_demo_window = true;
        //ImGui::ShowDemoWindow(&show_demo_window);

        // Mess :D
        glm::mat4 view = glm::lookAt(
            cameraPositionVector,
            cameraViewVector,
            glm::vec3(0.0f, 1.0f, 0.0f));

        // Basic Camera controls through DearImGUI
        {
            ImGui::Begin("Camera Controls");

            ImGui::SliderFloat3("Camera Position", &cameraPositionVector[0], 0.0f, 1000.0f);
            ImGui::SliderFloat3("Camera View", &cameraViewVector[0], 0.0f, 1000.0f);

            if (ImGui::Button("Reset Camera")) {
                cameraPositionVector = glm::vec3(200.0f, 200.0f, 200.0f);
                cameraViewVector = glm::vec3(150.0f, 150.0f, 100.0f);
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

        // Orthographic projection matrix for use in the Vertex Shader.
        //orthoProjection = glm::ortho(0.0f, (float)currentWindowWidth, 0.0f, (float)currentWindowHeight, -1.0f, 1.0f);
        perspectiveProjection = glm::perspective(90.0f, (float)currentWindowWidth / currentWindowHeight, 0.1f, 1000.0f);
        // Set MVP matrix once projection matrix has been updated.
        // Note that the calculation is actually Projection * View * Model as OpenGL uses column major ordering by default.
        // This affects how the MVP Matrix must be created.
        //mvpMatrix = orthoProjection * view * model;
        mvpMatrix = perspectiveProjection * view * model;
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