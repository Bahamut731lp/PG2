#pragma once

#include <glm/glm.hpp> 
#include <glm/ext.hpp>
#include <string>
#include <vector>

#include "Shader.h"
#include "Vertex.h"

/*
    - if texture_id is 0 it means that there is no texture.
*/
class Mesh {
public:
    // Mesh data
    std::vector<Vertex> vertices;
    std::vector<GLuint> indices;
    GLuint texture_id{ 0 };
    GLenum primitive_type = GL_TRIANGLES;

    // Mesh material
    glm::vec3 ambient{ 1.0f };
    glm::vec3 diffuse{ 1.0f };
    glm::vec3 specular{ 1.0f };
    float shininess = 1.0f;

    // Indirect (indexed) Draw 
    Mesh(GLenum primitive_type, std::vector<Vertex>& vertices, std::vector<GLuint>& indices, GLuint texture_id);

    void draw(Shader& shader);
    void clear();

private:
    // OpenGL buffer IDs
    // ID = 0 is reserved (i.e. uninitalized)
    unsigned int VAO, VBO, EBO;
};
