#pragma once
#include <GL/glew.h>
#include <GL/wglew.h>
#include <GLFW/glfw3.h>
#include "glm/glm.hpp"

struct PointLight {
	glm::vec3 position = glm::vec3(0.0f);
	glm::vec3 ambient = glm::vec3{ 0.0f };
	glm::vec3 diffusion = glm::vec3{ 10.0f };
	glm::vec3 specular = glm::vec3{ 1.0f };
	float intensity = 1.0f;
};

