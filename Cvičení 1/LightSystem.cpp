#pragma once
#include <glm/glm.hpp>
#include "LightSystem.h"
#include "Shader.h"
#include "PointLight.h"

void LightSystem::add(const PointLight& light)
{
    //podle n�jak�ho p�rka na StackOverflow je "emplace_back" lep�� ne� "push_back" ale kdo v�. Kr�sy 3. str�nky po google search
	lights.push_back(light);
}

void LightSystem::add(const Shader& shader)
{
    shaders.push_back(shader);
}

void LightSystem::calc()
{
    for (Shader shader : shaders) {
        for (const auto& light : lights) {
            shader.activate();
            shader.setUniform("light.position", light.position);
            shader.setUniform("light.ambient", light.ambient);
            shader.setUniform("light.diffuse", light.diffusion);
            shader.setUniform("light.specular", light.specular);
            shader.setUniform("light.constant", 1.0f);
            shader.setUniform("light.linear", 0.09f);
            shader.setUniform("light.quadratic", 0.032f);
        }
    }
}