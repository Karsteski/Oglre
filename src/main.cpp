// GLEW loads OpenGL function pointers from the system's graphics drivers.
// glew.h MUST be included before gl.h
#include <GL/glew.h>
#include <GL/gl.h>

// Disables inclusion of the dev-environ header.
// Allows GLFW + extension loader headers to be included in any order.
// GLFW including OpenGL headers causes function definition ambiguity.
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <vector>
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

    //glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    //glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    //glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* glfwMakeContextCurrent(window);
    << "GLFW Window creation failed\n"
    << "Exiting...\n";

    exit(EXIT_FAILURE);
}

// A valid OpenGL context must be created before initializing GLEW.
bool error = glewInit();
if (error != GLEW_OK) {
    std::cout << "Error: Failed to initialize OpenGL function pointer loader!\n";
}
// Initialize OpenGL loader (GLEW in this project)

// Printing OpenGL version for convenience
std::cout << "OpenGL Version + System GPU Drivers: " << glGetString(GL_VERSION) << std::endl;

std::vector<float> trianglePositions = {
    -0.5f, -0.5,
    0.0f, 0.5f,
    0.5f, -0.5f
};
// Main buffer
unsigned int buffer
    = 0;
glGenBuffers(1, &buffer);
glBindBuffer(GL_ARRAY_BUFFER, buffer);
glBufferData(GL_ARRAY_BUFFER, )

    // Main loop
    while (!glfwWindowShouldClose(window))
{
    glfwPollEvents();

    glClear(GL_COLOR_BUFFER_BIT);

    // Triangle just to test library
    glBegin(GL_TRIANGLES);

    glVertex2f(-0.5f, -0.5f);
    glVertex2f(0.0f, 0.5f);
    glVertex2f(0.5f, -0.5f);

    glEnd();
    // Swaps the front and back buffers of the specified window.
    glfwSwapBuffers(window);
}

glfwDestroyWindow(window);
glfwTerminate();

exit(EXIT_SUCCESS);
}