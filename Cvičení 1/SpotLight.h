#pragma once
#include <GL/glew.h>
#include <GL/wglew.h>
#include <GLFW/glfw3.h>
#include "glm/glm.hpp"

struct SpotLight {
    glm::vec3 position;
    glm::vec3 direction;
    float cutOff = glm::cos(glm::radians(20.5f));
    float outerCutOff = glm::cos(glm::radians(25.0f));

    float constant = 5.0f;
    float linear = 0.09f;
    float quadratic = 0.032f;

    glm::vec3 ambient = glm::vec3{ 0.0f };
    glm::vec3 diffusion = glm::vec3{ 10.0f };
    glm::vec3 specular = glm::vec3{ 1.0f };
};
