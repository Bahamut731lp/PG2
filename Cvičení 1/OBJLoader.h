#pragma once
#include "Loader.h"
#include "Mesh.h"

struct Material {
    std::string name;
    float ambient[3];
    float diffuse[3];
    float specular[3];
    float shininess;
};

/*
	Loader pro formát modelù .obj
*/
class OBJLoader : public Loader
{
public:
    OBJLoader(const std::filesystem::path& filename);
	Mesh getMesh();

    class Parse {
    public:
        static void vertex(std::vector<std::string> input, std::vector<glm::vec3>& output);
        static void uv(std::vector<std::string> input, std::vector<glm::vec2>& output);
        static void normal(std::vector<std::string> input, std::vector<glm::vec3>& output);
        static void face(std::vector<std::string> input, std::vector<unsigned int>& vertices_i, std::vector<unsigned int>& uvs_i, std::vector<unsigned int>& normals_i);
        static std::vector<Material> MTL(std::vector<std::string> input);
    };
};

