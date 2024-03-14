#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Shader.h"

void Shader::setUniform(const std::string& name, const float val)
{
	auto loc = glGetUniformLocation(ID, name.c_str());
	if (loc == -1) {
		std::cerr << "no uniform with name:" << name << '\n';
		return;
	}

	glUniform1f(loc, val);
}

void Shader::setUniform(const std::string& name, const glm::vec4 val)
{
	auto loc = glGetUniformLocation(ID, name.c_str());
	if (loc == -1) {
		std::cerr << "no uniform with name:" << name << '\n';
		return;
	}
	glUniform4fv(loc, 1, glm::value_ptr(val));
}

void Shader::setUniform(const std::string& name, const glm::mat4 val)
{
	auto loc = glGetUniformLocation(ID, name.c_str());
	if (loc == -1) {
		std::cout << "no uniform with name:" << name << '\n';
		return;
	}

	glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(val));
}
