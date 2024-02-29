#pragma once
#include <GLFW/glfw3.h>

class WindowManager
{
public:
	WindowManager(int, int, const char*);
	WindowManager(GLFWwindow*);
	void setToFullscreenMode();
	void setToWindowedMode();
	GLFWwindow* getWindow();

private:
	GLFWwindow* window;
};

