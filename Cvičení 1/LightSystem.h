#pragma once

#include <vector>
#include <glm/glm.hpp>
#include "Shader.h"
#include "PointLight.h"


class LightSystem {
public:
    LightSystem() = default;

    void add(const PointLight& light);
    void add(const Shader& shader);
    void calc();

private:
    std::vector<PointLight> lights;
    std::vector<Shader> shaders;
};