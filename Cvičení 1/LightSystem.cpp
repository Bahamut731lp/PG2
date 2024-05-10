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
        int index = 0;
        for (const auto& light : lights) {
            shader.activate();
            //TODO: Convert this to UBOs
            shader.setUniform("pointLights[" + std::to_string(index) + "].position", light.position);
            shader.setUniform("pointLights[" + std::to_string(index) + "].ambient", light.ambient);
            shader.setUniform("pointLights[" + std::to_string(index) + "].diffuse", light.diffusion);
            shader.setUniform("pointLights[" + std::to_string(index) + "].specular", light.specular);
            shader.setUniform("pointLights[" + std::to_string(index) + "].constant", 1.0f);
            shader.setUniform("pointLights[" + std::to_string(index) + "].linear", 0.09f);
            shader.setUniform("pointLights[" + std::to_string(index) + "].quadratic", 0.032f);

            index += 1;
        }
    }
}