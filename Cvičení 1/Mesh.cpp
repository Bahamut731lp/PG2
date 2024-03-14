#include "Mesh.h"

Mesh::Mesh(GLenum primitive_type, std::vector<Vertex>& vertices, std::vector<GLuint>& indices, GLuint texture_id) {

    // TODO: create and initialize VAO, VBO, EBO and parameters
}

void Mesh::draw(Shader& shader)
{

    // TODO: set uniform variables: color

    //set texture id etc...
    //if (texture_id > 0) {
    //    glActiveTexture(GL_TEXTURE0);
    //    glBindTexture(GL_TEXTURE_2D, texture_id);
    //    shader.setUniform("mytexture", 0);
    //}

    //TODO: draw mesh: bind vertex array object, draw all elements with selected primitive type, unbind vertex array object 

}

void Mesh::clear()
{
    vertices.clear();
    indices.clear();
    texture_id = 0;
    primitive_type = GL_POINT;

    // TODO: delete all allocations 
    //glDeleteBuffers...
    //    glDeleteVertexArrays...
}
