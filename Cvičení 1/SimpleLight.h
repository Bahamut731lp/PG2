#pragma once
#include <GL/glew.h>
#include <GL/wglew.h>
#include <GLFW/glfw3.h>
#include "glm/glm.hpp"


#include "Camera.h"
#include "Model.h"
#include "Mesh.h"
#include "OBJLoader.h"
#include "Shader.h"


class SimpleLight
{
public:
	glm::vec3 position;
	glm::vec3 ambient{ 0.2f };
	glm::vec3 diffusion{ 5.0f };
	glm::vec3 specular{ 1.0f };
	float intensity;

	SimpleLight(glm::vec3 position, float intensity);
	void render(Camera& camera);

private:
	Shader shader{ "./assets/shaders/light.vert", "./assets/shaders/light.frag" };
	Mesh mesh = Model{ "./assets/obj/cube_triangles.obj" }.meshes.front();
};

