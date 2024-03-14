#pragma once

#include <filesystem>
#include <set>

#include "Shader.h"
#include "Vertex.h"
#include "Mesh.h"

class Model
{
public:

    Model(const std::filesystem::path& filename) {

        // Koncovna nen� v seznamu podporovan�ch form�t�
        // Tak�e to mus�me n�jak po��fit
        if (Model::SUPPORTED_FORMATS.count(filename.extension().string()) == 0) {
            //TODO: Po��fit
        }


        // load mesh (all meshes) of the model, load material of each mesh, load textures...
        // TODO: call LoadOBJFile, LoadMTLFile, process data, create mesh and set its properties 
    }

    void draw(const Shader& shader);

private:
    std::vector<Mesh> meshes;
    std::string name;
    static const std::set<std::string> SUPPORTED_FORMATS;

    void loadOBJFile(const std::filesystem::path& filename); //TODO: modify
    void LoadMTLFile(const std::filesystem::path& filename); //TODO: implement

    //GLuint LoadTexture(const std::filesystem::pathg& filename);

    //...
}