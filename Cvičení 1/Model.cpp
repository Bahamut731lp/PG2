#pragma once
#include <string>

#include <GL/glew.h>
#include <GL/wglew.h>
#include <GLFW/glfw3.h>
#include "glm/glm.hpp"

#include "Model.h"
#include "Logger.h"
#include "OBJLoader.h"

Model::Model(const std::filesystem::path& filename)
{
	this->transform = glm::mat4(1.0f);
	this->meshes = std::vector<Mesh>{};

	auto suffix = filename.extension().string();
	Logger::debug(suffix);

	if (suffix == ".obj") {
		auto loader = OBJLoader(filename);

		for (Mesh mesh : loader.submeshes) {
			meshes.push_back(mesh);
		}
	}
	else {
		Logger::error("Unrecognized file extension: " + suffix);
	};
}

Model::Model(const Model& copy)
{
	this->meshes = copy.meshes;
	this->transform = glm::mat4(1.0f);
}

void Model::render(Camera& camera, Shader& shader)
{
	shader.activate();
	//transform = glm::mat4(1.0f);
	shader.setUniform("transform", transform);
	shader.setUniform("view", camera.getViewMatrix());
	shader.setUniform("projection", camera.getProjectionMatrix());

	for (Mesh mesh : meshes)
	{
		shader.setUniform("material.ambient", mesh.material.ambient);
		shader.setUniform("material.diffuse", mesh.material.diffuse);
		shader.setUniform("material.specular", mesh.material.specular);
		shader.setUniform("material.shininess", mesh.material.shininess);

		mesh.draw(shader);

		//Logger::warning(mesh.material.name + " (" + std::to_string(mesh.material.diffuse[0]) + ", " + std::to_string(mesh.material.diffuse[1]) + ", " + std::to_string(mesh.material.diffuse[2]) + ")");
	}
}
