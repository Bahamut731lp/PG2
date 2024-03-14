#include "Model.h"

const std::set<std::string> Model::SUPPORTED_FORMATS = { ".obj" };

void Model::draw(const Shader& shader)
{
    for (auto const& mesh : meshes) {
        mesh.draw(shader);
    }
}

void Model::loadOBJFile(const std::filesystem::path& filename)
{
}
