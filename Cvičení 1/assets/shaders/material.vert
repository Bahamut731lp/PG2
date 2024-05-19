#version 460 core

struct Texture {
    sampler2D textureUnit;
    int isTextured;
    vec3 scale;
};

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;    
    float shininess;
    float transparency;
    Texture texture;
}; 

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;

out vec3 FragPos;
out vec3 Normal;
out vec2 TexCoord;

uniform mat4 transform;
uniform mat4 view;
uniform mat4 projection;
uniform Material material;

void main()
{
    FragPos = vec3(transform * vec4(aPos, 1.0));
    Normal = mat3(transpose(inverse(transform))) * aNormal;

    if (material.texture.isTextured == 1) {
        TexCoord = vec2(aTexCoord.x * material.texture.scale.x, aTexCoord.y * material.texture.scale.y);
    } else {
        TexCoord = vec2(aTexCoord.x, aTexCoord.y); 
    }
    
    gl_Position = projection * view * vec4(FragPos, 1.0);
}