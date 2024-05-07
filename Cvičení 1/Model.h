#pragma once
#include <filesystem>
#include <vector>

#include <GL/glew.h>
#include <GL/wglew.h>
#include <GLFW/glfw3.h>
#include "glm/glm.hpp"

#include "Camera.h"
#include "Mesh.h"

class Model
{
public:
	glm::mat4 transform;
	std::vector<Mesh> meshes;

	Model(const std::filesystem::path& filename);
	Model(const Model& copy);

	void render(Camera& camera, Shader& shader);
};

