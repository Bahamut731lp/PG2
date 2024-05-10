#pragma once

#include <vector>
#include <string>
#include <unordered_map>
#include <glm/glm.hpp>
#include "Shader.h"

#include "PointLight.h"
#include "SpotLight.h"
#include "DirectionalLight.h"

class LightSystem {
public:
    std::vector<PointLight*> pointLights;
    std::vector<SpotLight*> spotLights;
    std::vector<DirectionalLight*> directionalLights;

    LightSystem() = default;

    void add(PointLight& light);
    void add(SpotLight& light);
    void add(DirectionalLight& light);
    void add(Shader& shader);
    void calc();

private:
    std::vector<Shader> shaders;
};