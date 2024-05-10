#pragma once
#include <glm/glm.hpp>
#include "LightSystem.h"
#include "Shader.h"
#include "PointLight.h"

void LightSystem::add(const PointLight& light)
{
    //podle nìjakýho párka na StackOverflow je "emplace_back" lepší než "push_back" ale kdo ví. Krásy 3. stránky po google search
	lights.push_back(light);
}

void LightSystem::add(const SpotLight& light)
{
    spotLights.push_back(light);
}

void LightSystem::add(const Shader& shader)
{
    shaders.push_back(shader);
}

void LightSystem::calc()
{
    for (Shader shader : shaders) {
        int index = 0;
        //TODO: Convert this part to UBOs
        for (const auto& light : lights) {
            shader.activate();
            shader.setUniform("pointLights[" + std::to_string(index) + "].position", light.position);
            shader.setUniform("pointLights[" + std::to_string(index) + "].ambient", light.ambient);
            shader.setUniform("pointLights[" + std::to_string(index) + "].diffuse", light.diffusion);
            shader.setUniform("pointLights[" + std::to_string(index) + "].specular", light.specular);
            shader.setUniform("pointLights[" + std::to_string(index) + "].constant", 1.0f);
            shader.setUniform("pointLights[" + std::to_string(index) + "].linear", 0.09f);
            shader.setUniform("pointLights[" + std::to_string(index) + "].quadratic", 0.032f);

            index += 1;
        }

        index = 0;
        //TODO: Convert this part to UBOs
        for (const auto& light : spotLights) {
            shader.activate();
            shader.setUniform("spotLights[" + std::to_string(index) + "].position", light.position);
            shader.setUniform("spotLights[" + std::to_string(index) + "].direction", light.direction);
            shader.setUniform("spotLights[" + std::to_string(index) + "].ambient", light.ambient);
            shader.setUniform("spotLights[" + std::to_string(index) + "].diffuse", light.diffusion);
            shader.setUniform("spotLights[" + std::to_string(index) + "].specular", light.specular);
            shader.setUniform("spotLights[" + std::to_string(index) + "].constant", light.constant);
            shader.setUniform("spotLights[" + std::to_string(index) + "].linear", light.linear);
            shader.setUniform("spotLights[" + std::to_string(index) + "].quadratic", light.quadratic);
            shader.setUniform("spotLights[" + std::to_string(index) + "].cutOff", light.cutOff);
            shader.setUniform("spotLights[" + std::to_string(index) + "].outerCutOff", light.outerCutOff);

            index += 1;
        }
    }
}