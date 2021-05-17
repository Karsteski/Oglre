// GLEW loads OpenGL function pointers from the system's graphics drivers.
#include <GL/glew.h>

// Disables inclusion of the dev-environ header.
// Allows GLFW + extension loader headers to be included in any order.
// GLFW including OpenGL headers causes function definition ambiguity.
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <iostream>

const int initialWindowWidth = 800;
const int initialWindowHeight = 600;

int main()
{
    // GLFW Setup
    if (!glfwInit()) {
        std::cout << "GLFW Initialization failed!\n"
                  << "Exiting...\n";

        exit(EXIT_FAILURE);
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(initialWindowWidth, initialWindowHeight, "Oglre", NULL, NULL);
    if (!window) {
        std::cout << "GLFW Window creation failed\n"
                  << "Exiting...\n";

        exit(EXIT_FAILURE);
    }

    glfwMakeContextCurrent(window);
    bool error = glewInit();
    if (error != GLEW_OK) {
        std::cout << "Error: Failed to initialize OpenGL function pointer loader!\n";
    }
    // Initialize OpenGL loader (GLEW in this project)

    // Main loop
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();

    exit(EXIT_SUCCESS);
}