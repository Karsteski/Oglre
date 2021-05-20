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

#include <array>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

// Globals
const int initialWindowWidth = 800;
const int initialWindowHeight = 600;
std::string shaderPath = "../resources/shaders/Basic.shader";

static void GLClearError()
{
    // Clear previous errors.
    while (glGetError() != GL_NO_ERROR) {
        continue;
    };
}

static void GLCheckError()
{
    // Run until no more errors to catch.
    while (GLenum error = glGetError()) {
        std::cout << "[OpenGL Error] (" << error << ")" << std::endl;
    }
}

// Takes care of returning the two strings from parseShader().
struct ShaderProgramSource {
    std::string vertexSource;
    std::string fragmentSource;
};

static ShaderProgramSource parseShader(const std::string& filepath)
{
    // For separating the two stringstreams.
    enum class ShaderType {
        NONE = -1,
        VERTEX = 0,
        FRAGMENT = 1
    };

    std::ifstream stream(filepath);
    std::array<std::stringstream, 2> ss;

    std::string line = "";
    ShaderType type = ShaderType::NONE;

    // Read lines from the file while separating the two shader types into different stringstreams.
    while (getline(stream, line)) {
        if (line.find("#shader") != std::string::npos) {
            if (line.find("vertex") != std::string::npos) {
                type = ShaderType::VERTEX;
            } else if (line.find("fragment") != std::string::npos) {
                type = ShaderType::FRAGMENT;
            }
        } else {
            ss[static_cast<int>(type)] << line << "\n";
        }
    }

    ShaderProgramSource shaders;
    shaders.vertexSource = ss[0].str();
    shaders.fragmentSource = ss[1].str();

    return shaders;
}

// Ensure that source string does not go out of scope before running compileShader().
static unsigned int compileShader(unsigned int shaderType, const std::string& source)
{
    unsigned int id = glCreateShader(shaderType);

    // Note that source has to exist
    const char* src = source.c_str();

    const int numberOfShaderSources = 1;
    // Note that strings must be null-terminated if passing length as nullptr.
    glShaderSource(id, numberOfShaderSources, &src, nullptr);
    glCompileShader(id);

    // Error handling.
    int result = 0;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);
    if (result == GL_FALSE) {
        int errorMessageLength = 0;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &errorMessageLength);
        std::string message = "";
        glGetShaderInfoLog(id, errorMessageLength, &errorMessageLength, message.data());

        // A bit hacky, will eventually need proper logging.
        std::cout << "Failed to compile " << (shaderType == GL_VERTEX_SHADER ? "vertex" : "fragment") << " shader!\n"
                  << message << std::endl;

        glDeleteShader(id);

        return 0;
    }

    return id;
}

// Returns ID for a shader that combines the vertexShader and the fragmentShader.
static unsigned int createShader(const std::string& vertexShader, const std::string& fragmentShader)
{
    // Need error handling.
    unsigned int program = glCreateProgram();
    unsigned int vs = compileShader(GL_VERTEX_SHADER, vertexShader);
    unsigned int fs = compileShader(GL_FRAGMENT_SHADER, fragmentShader);

    // These steps create an executable that is run on the programmable vertex/fragment shader processer on the GPU.
    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);
    glValidateProgram(program);

    // Delete to shaders once they have been linked and compiled.
    glDeleteShader(vs);
    glDeleteShader(fs);

    return program;
}

int main()
{
    // C++ version verification.
    std::cout << "C++ version = ";
    if (__cplusplus == 201703L)
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

    // I may just remove these, I can't get anything drawn with them set.
    // glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    // glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
    // glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    // glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    GLFWwindow* window = glfwCreateWindow(initialWindowWidth, initialWindowHeight, "Oglre", NULL, NULL);
    if (!window) {
        std::cout << "GLFW Window creation failed\n"
                  << "Exiting...\n";

        exit(EXIT_FAILURE);
    }

    glfwMakeContextCurrent(window);

    // A valid OpenGL context must be created before initializing GLEW.
    bool error = glewInit();
    if (error != GLEW_OK) {
        std::cout << "Error: Failed to initialize OpenGL function pointer loader!\n";
    }
    // Initialize OpenGL loader (GLEW in this project).

    // Printing OpenGL version for convenience.
    std::cout << "OpenGL Version + System GPU Drivers: " << glGetString(GL_VERSION) << std::endl;

    // clang-format off
    std::vector<float> positions = {
        -0.5f, -0.5f,    
        0.5f, -0.5f,
        0.5f, 0.5f,
        -0.5f, 0.5f,
    };
    // clang-format on

    // Index Buffer
    // References the vector of positions necessary to draw two triangles making up a square.
    std::vector<unsigned int> indices {
        0, 1, 2,
        2, 3, 0
    };

    // Main buffer
    // These should be moved to a struct eventually.
    unsigned int buffer = 0;
    const int numberOfBuffers = 1;
    const int numberOfPoints = 6 * 2;
    const int numberOfIndices = 6; // Note that indices are the vertices in the postions vector, specifed by 2 points.

    glGenBuffers(numberOfBuffers, &buffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glBufferData(GL_ARRAY_BUFFER, numberOfPoints * sizeof(float), positions.data(), GL_STATIC_DRAW);

    // Generate Index Buffer.
    unsigned int indexBufferObject = 0;
    glGenBuffers(numberOfBuffers, &indexBufferObject);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferObject);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, numberOfIndices * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

    // Should move these to a struct eventually.
    const int attributeIndex = 0; // Attribute in this case refers to the position coordinates.
    const int componentCount = 2; // This is the number of floats representing the position coordinates attribute.
    const int stride = 2 * sizeof(float); // Number of bytes between each vertex.
    const void* offset = 0; // Long explanation, see: http://docs.gl/gl4/glVertexAttribPointer

    glVertexAttribPointer(attributeIndex, componentCount, GL_FLOAT, GL_FALSE, stride, offset);
    // Must enable the generic vertex attribute array for the vertex to be drawn.
    glEnableVertexAttribArray(attributeIndex);

    // Shader creation.
    ShaderProgramSource shaderSource = parseShader(shaderPath);
    unsigned int shader = createShader(shaderSource.vertexSource, shaderSource.fragmentSource);
    glUseProgram(shader);

    // Main loop
    while (!glfwWindowShouldClose(window)) {

        // Render from this point on.
        glClear(GL_COLOR_BUFFER_BIT);

        // Index Buffer is already bound, and so the pointer to the IB location can just be nullptr.
        glDrawElements(GL_TRIANGLES, numberOfIndices, GL_UNSIGNED_INT, nullptr);

        // Swaps the front and back buffers of the specified window.
        // The front buffer is the current buffer shown on screen, whilst the back is the data to be drawn to.
        glfwSwapBuffers(window);

        // Poll and process events.
        glfwPollEvents();
    }

    glDeleteProgram(shader);
    glfwDestroyWindow(window);
    glfwTerminate();

    exit(EXIT_SUCCESS);
}