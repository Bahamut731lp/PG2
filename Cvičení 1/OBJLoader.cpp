#pragma once
#include "Vertex.h"
#include "Mesh.h"
#include "OBJLoader.h"

OBJLoader::OBJLoader(const std::filesystem::path& filename)
{
	std::vector< unsigned int > vertexIndices, uvIndices, normalIndices;
	std::vector< glm::vec3 > temp_vertices;
	std::vector< glm::vec2 > temp_uvs;
	std::vector< glm::vec3 > temp_normals;

	std::cout << "Loading model: " << filename.string() << std::endl;

	FILE* file;
	fopen_s(&file, filename.string().c_str(), "r");

	// Kontrola existence souboru
	if (file == NULL) {
		printf("Impossible to open the file !\n");
		return;
	}

	const int MAX_LINE_SIZE = 255;

	while (1) {
		char lineHeader[MAX_LINE_SIZE];
		int res = fscanf_s(file, "%s", lineHeader, MAX_LINE_SIZE);

		if (res == EOF) {
			break;
		}

		if (strcmp(lineHeader, "v") == 0) {
			glm::vec3 vertex;
			fscanf_s(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z);
			temp_vertices.push_back(vertex);
		}
		else if (strcmp(lineHeader, "vt") == 0) {
			glm::vec2 uv;
			fscanf_s(file, "%f %f\n", &uv.y, &uv.x);
			temp_uvs.push_back(uv);
		}
		else if (strcmp(lineHeader, "vn") == 0) {
			glm::vec3 normal;
			fscanf_s(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z);
			temp_normals.push_back(normal);
		}
		else if (strcmp(lineHeader, "f") == 0) {
			std::string vertex1, vertex2, vertex3;
			unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
			int matches = fscanf_s(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &vertexIndex[0], &uvIndex[0], &normalIndex[0], &vertexIndex[1], &uvIndex[1], &normalIndex[1], &vertexIndex[2], &uvIndex[2], &normalIndex[2]);
			if (matches != 9) {
				printf("File can't be read by simple parser :( Try exporting with other options\n");
				return;
			}
			vertexIndices.push_back(vertexIndex[0]);
			vertexIndices.push_back(vertexIndex[1]);
			vertexIndices.push_back(vertexIndex[2]);
			uvIndices.push_back(uvIndex[0]);
			uvIndices.push_back(uvIndex[1]);
			uvIndices.push_back(uvIndex[2]);
			normalIndices.push_back(normalIndex[0]);
			normalIndices.push_back(normalIndex[1]);
			normalIndices.push_back(normalIndex[2]);
		}
	}

	// unroll from indirect to direct vertex specification
	// sometimes not necessary, definitely not optimal
	for (unsigned int u = 0; u < vertexIndices.size(); u++) {
		unsigned int vertexIndex = vertexIndices[u];
		glm::vec3 vertex = temp_vertices[vertexIndex - 1];
		vertices.push_back(vertex);
	}
	for (unsigned int u = 0; u < uvIndices.size(); u++) {
		unsigned int uvIndex = uvIndices[u];
		glm::vec2 uv = temp_uvs[uvIndex - 1];
		uvs.push_back(uv);
	}
	for (unsigned int u = 0; u < normalIndices.size(); u++) {
		unsigned int normalIndex = normalIndices[u];
		glm::vec3 normal = temp_normals[normalIndex - 1];
		normals.push_back(normal);
	}

	// Iterate through the vertices and assign indices sequentially
	for (unsigned int i = 0; i < vertices.size(); ++i) {
		indices.push_back(i);
	}

	fclose(file);

	//TODO: Cover the situation that these sizes are not the same
	std::cout << "V: " << vertices.size() << std::endl;
	std::cout << "U: " << uvs.size() << std::endl;
	std::cout << "N: " << normals.size() << std::endl;
}

Mesh OBJLoader::getMesh()
{
	//Hey, i know it's spelled wrong.
	//But I've vertices already defined.
	std::vector<Vertex> vertexes;

	for (unsigned int i = 0; i < vertices.size(); i++) {
		Vertex vertex;
		vertex.Position = vertices[i];
		vertex.UVs = uvs[i];
		vertex.Normal = normals[i];

		vertexes.push_back(vertex);
	}

	std::cout << "V: " << vertices.size() << "\tI: " << indices.size() << std::endl;
	return Mesh(GL_TRIANGLES, vertexes, indices, 0);
}
