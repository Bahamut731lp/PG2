#pragma once
#include <GLFW/glfw3.h>

class InputManager
{
public:
	static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
private:
	static void handleKeyPress(int key, int action, GLFWwindow* window);
};

