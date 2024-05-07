#pragma once
#include <glm/glm.hpp>
#include "LightSystem.h"
#include "Shader.h"
#include "SimpleLight.h"

void LightSystem::add(const SimpleLight& light)
{
    //podle nìjakýho párka na StackOverflow je "emplace_back" lepší než "push_back" ale kdo ví. Krásy 3. stránky po google search
	lights.push_back(light);
}

void LightSystem::calc(Shader& meshShader, Shader& terrainShader)
{
    for (const auto& light : lights) {
        // Moving light
        meshShader.activate();
        meshShader.setUniform("light.position", light.position);
        meshShader.setUniform("light.ambient", light.ambient);
        meshShader.setUniform("light.diffuse", light.diffusion);
        meshShader.setUniform("light.specular", light.specular);

        terrainShader.activate();
        terrainShader.setUniform("light.position", light.position);
        terrainShader.setUniform("light.ambient", light.ambient);
        terrainShader.setUniform("light.diffuse", light.diffusion);
        terrainShader.setUniform("light.specular", light.specular);
    }
}