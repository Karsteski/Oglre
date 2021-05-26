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