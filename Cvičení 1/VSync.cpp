#include "VSync.h"
#include <GLFW/glfw3.h>

VSync& VSync::getInstance()
{
	static VSync instance;
	return instance;
}

void VSync::enable()
{
	glfwSwapInterval(1);
	isEnabled = true;
}

void VSync::disable()
{
	glfwSwapInterval(0);
	isEnabled = false;
}
