#include "AuxiliaryFunctions.h"

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

float Oglre::getDeltaTime()
{
    static float deltaTime = 0.0f;
    static float currentTime = 0.0f;
    static float lastTime = 0.0f;

    currentTime = glfwGetTime();
    deltaTime = currentTime - lastTime;
    lastTime = currentTime;

    return deltaTime;
}


