// GLEW loads OpenGL function pointers from the system's graphics drivers.
// glew.h MUST be included before gl.h
// clang-format off
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
//const int initialWindowWidth = 800;
//const int initialWindowHeight = 600;
std::string shaderPath = "../resources/shaders/Basic.glsl";

int main()
{
    Oglre::Application::Initialize();
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

        // DearImGUI things
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // Flags
        static int f_Projection = 0;

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
        Oglre::Application::ProcessKeyboardInput(window);

        // Process mouse input and movement.
        glfwSetMouseButtonCallback(window, Oglre::Application::MouseButtonCallback);
        glfwSetCursorPosCallback(window, Oglre::Application::MouseMovementCallback);
        glfwSetScrollCallback(window, Oglre::Application::MouseScrollWheelCallback);

        // Projection matrix for use in the Vertex Shader.
        if (f_Projection == 0) {
            projection = glm::perspective(glm::radians(camera.cameraFOV), (float)currentWindowWidth / currentWindowHeight, 0.1f, 10000.0f);
        } else if (f_Projection == 1) {
            // TODO: Works, but I need the object to be in the same position when switching between perspectives (Possible..?)
            static float min = -pow(10, glm::radians(camera.cameraFOV));
            static float max = pow(10, glm::radians(camera.cameraFOV));

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

    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();

    exit(EXIT_SUCCESS);
}