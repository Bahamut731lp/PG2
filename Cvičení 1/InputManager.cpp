#include "InputManager.h"
#include "VSync.h"
#include <iostream>

void InputManager::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    switch (action) {
    case GLFW_PRESS:
        InputManager::handleKeyPress(key, action, window);
        break;
    default:
        return;
    }
    
}

void InputManager::handleKeyPress(int key, int action, GLFWwindow* window)
{
    switch (key) {
    case GLFW_KEY_ESCAPE:
        glfwSetWindowShouldClose(window, GLFW_TRUE);
        break;
    case GLFW_KEY_V:
        VSync& vsync = VSync::getInstance();
        vsync.isEnabled ? vsync.disable() : vsync.enable();

        std::cout << "VSync " << (vsync.isEnabled ? "enabled" : "disabled") << std::endl;

        break;
    }
}
