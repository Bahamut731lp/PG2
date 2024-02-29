#include "WindowManager.h"
#include <string>

WindowManager::WindowManager(int width, int height, const char* title)
{
	window = glfwCreateWindow(width, height, title, NULL, NULL);
	glfwMakeContextCurrent(window);
}

WindowManager::WindowManager(GLFWwindow* instance) 
{
	window = instance;
	glfwMakeContextCurrent(window);
}

void WindowManager::setToFullscreenMode()
{
	GLFWmonitor* monitor = glfwGetPrimaryMonitor();
	const GLFWvidmode* mode = glfwGetVideoMode(monitor);

	glfwSetWindowMonitor(window, monitor, 0, 0, mode->width, mode->height, mode->refreshRate);
}

void WindowManager::setToWindowedMode()
{
	glfwSetWindowMonitor(window, nullptr, 0, 0, 640, 480, 0);
}

GLFWwindow* WindowManager::getWindow()
{
	return window;
}
