#pragma once

#include <vector>
#include <glm/glm.hpp>
#include "Shader.h"
#include "SimpleLight.h"


class LightSystem {
public:
    LightSystem() = default;

    void add(const SimpleLight& light);
    void calc(Shader& meshShader, Shader& terrainShader);

private:
    std::vector<SimpleLight> lights;
};