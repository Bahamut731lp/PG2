#pragma once
#include <glm/glm.hpp>
#include "LightSystem.h"
#include "Shader.h"
#include "PointLight.h"

void LightSystem::add(AmbientLight* light)
{
    ambientLights.push_back(light);
}

void LightSystem::add(PointLight* light)
{
    pointLights.push_back(light);
}

void LightSystem::add(SpotLight* light)
{
    spotLights.push_back(light);
}

void LightSystem::add(DirectionalLight* light)
{
    directionalLights.push_back(light);
}

void LightSystem::add(Shader& shader)
{
    shaders.push_back(shader);
}

void LightSystem::calc()
{
    for (Shader shader : shaders) {
        int index = 0;
        //TODO: Convert this part to UBOs

        for (auto light : pointLights) {
            shader.activate();
            shader.setUniform("pointLights[" + std::to_string(index) + "].position", light->position);
            shader.setUniform("pointLights[" + std::to_string(index) + "].ambient", light->ambient);
            shader.setUniform("pointLights[" + std::to_string(index) + "].diffuse", light->diffusion);
            shader.setUniform("pointLights[" + std::to_string(index) + "].specular", light->specular);
            shader.setUniform("pointLights[" + std::to_string(index) + "].constant", 1.0f);
            shader.setUniform("pointLights[" + std::to_string(index) + "].linear", 0.09f);
            shader.setUniform("pointLights[" + std::to_string(index) + "].quadratic", 0.032f);

            index += 1;
        }

        index = 0;
        //TODO: Convert this part to UBOs
        for (const auto& light : spotLights) {
            shader.activate();
            shader.setUniform("spotLights[" + std::to_string(index) + "].position", light->position);
            shader.setUniform("spotLights[" + std::to_string(index) + "].direction", light->direction);
            shader.setUniform("spotLights[" + std::to_string(index) + "].ambient", light->ambient);
            shader.setUniform("spotLights[" + std::to_string(index) + "].diffuse", light->diffusion);
            shader.setUniform("spotLights[" + std::to_string(index) + "].specular", light->specular);
            shader.setUniform("spotLights[" + std::to_string(index) + "].constant", light->constant);
            shader.setUniform("spotLights[" + std::to_string(index) + "].linear", light->linear);
            shader.setUniform("spotLights[" + std::to_string(index) + "].quadratic", light->quadratic);
            shader.setUniform("spotLights[" + std::to_string(index) + "].cutOff", light->cutOff);
            shader.setUniform("spotLights[" + std::to_string(index) + "].outerCutOff", light->outerCutOff);

            index += 1;
        }

        index = 0;
        //TODO: Convert this part to UBOs
        for (const auto& light : directionalLights) {
            shader.activate();
            shader.setUniform("directionalLights[" + std::to_string(index) + "].direction", light->direction);
            shader.setUniform("directionalLights[" + std::to_string(index) + "].ambient", light->ambient);
            shader.setUniform("directionalLights[" + std::to_string(index) + "].diffuse", light->diffusion);
            shader.setUniform("directionalLights[" + std::to_string(index) + "].specular", light->specular);

            index += 1;
        }

        index = 0;
        //TODO: Convert this part to UBOs
        for (const auto& light : ambientLights) {
            shader.activate();
            shader.setUniform("ambientLights[" + std::to_string(index) + "].color", light->color);
            shader.setUniform("ambientLights[" + std::to_string(index) + "].intensity", light->intensity);

            index += 1;
        }
    }
}