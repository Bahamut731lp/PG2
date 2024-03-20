#pragma once
#include "Loader.h"
#include "Mesh.h"

/*
	Loader pro form�t model� .obj
*/
class OBJLoader : public Loader
{
public:
	OBJLoader(const std::filesystem::path& filename);
	Mesh getMesh();
};

