#pragma once

#include <vector>
#include <glm/glm.hpp>
#include "Shader.h"
#include "SimpleLight.h"


class LightSystem {
public:
    LightSystem() = default;

    void add(const SimpleLight& light);
    void add(const Shader& shader);
    void calc();

private:
    std::vector<SimpleLight> lights;
    std::vector<Shader> shaders;
};