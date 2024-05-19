#pragma once

#include <glm/glm.hpp>

struct Texture {
    int id = -1;
    glm::vec3 scale = glm::vec3(5.0f);
};

struct Material {
    std::string name;
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
    float transparency = 1.0f;
    float shininess;

    Texture texture;
};