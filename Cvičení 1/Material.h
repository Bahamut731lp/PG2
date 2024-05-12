#pragma once

#include <glm/glm.hpp>

struct Material {
    std::string name;
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
    float transparency = 1.0f;
    float shininess;
};