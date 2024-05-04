#pragma once
#include <sstream>
#include "Logger.h"
#include "Vertex.h"
#include "Mesh.h"
#include "OBJLoader.h"
#include "StringUtils.h"

OBJLoader::OBJLoader(const std::filesystem::path& filename)
{
	std::vector< unsigned int > vertexIndices, uvIndices, normalIndices;
	std::vector<glm::vec3> temp_vertices;
	std::vector<glm::vec2> temp_uvs;
	std::vector<glm::vec3> temp_normals;

	Logger::info("Loading " + filename.string());

	std::ifstream file(filename);
	if (!file.is_open()) {
		Logger::error("File " + filename.string() + " does not exist!");
		return;
	}

	std::string line;
	while (std::getline(file, line)) {
		std::vector<std::string> tokens = split(line, ' ');

		// If the line is empty for some reason, we skip it.
		// This should not happen, but better be safe than sorry.
		if (tokens.empty()) continue;

		// Get the line header
		std::string header = tokens.front();
		// Remove it from the token list
		tokens.erase(tokens.begin());
		
		// Comments
		if (header == "#") continue;
		// Texture definition
		else if (header == "mtllib") OBJLoader::Parse::MTL(tokens);
		// Vertices definition
		else if (header == "v") OBJLoader::Parse::vertex(tokens, temp_vertices);
		// UVs definition
		else if (header == "vt") OBJLoader::Parse::uv(tokens, temp_uvs);
		// Normals definition
		else if (header == "vn") OBJLoader::Parse::normal(tokens, temp_normals);
		// Faces
		else if (header == "f") OBJLoader::Parse::face(tokens, vertexIndices, uvIndices, normalIndices);
	}

	std::ostringstream statistics, indicies;

	Logger::info("Parsed model " + filename.string());

	statistics << "Vertices: " << temp_vertices.size() << ", ";
	statistics << "UVs: " << temp_uvs.size() << ", ";
	statistics << "Normals: " << temp_normals.size();

	indicies << "Vertex Indicies: " << vertexIndices.size() << ", ";
	indicies << "UV Indicies: " << uvIndices.size() << ", ";
	indicies << "Normal Indicies: " << normalIndices.size();

	Logger::debug("Model Stats: " + statistics.str());
	Logger::debug("Model Indicies: " + indicies.str());

	// unroll from indirect to direct vertex specification
	// sometimes not necessary, definitely not optimal
	for (unsigned int u = 0; u < vertexIndices.size(); u++) {
		unsigned int vertexIndex = vertexIndices[u];
		glm::vec3 vertex = temp_vertices[vertexIndex - 1];
		vertices.push_back(vertex);
	}

	if (!temp_uvs.empty()) {
		for (unsigned int u = 0; u < uvIndices.size(); u++) {
			unsigned int uvIndex = uvIndices[u];
			glm::vec2 uv = temp_uvs[uvIndex - 1];
			uvs.push_back(uv);
		}
	}

	if (!temp_normals.empty()) {
		for (unsigned int u = 0; u < normalIndices.size(); u++) {
			unsigned int normalIndex = normalIndices[u];
			glm::vec3 normal = temp_normals[normalIndex - 1];
			normals.push_back(normal);
		}
	}

	// Iterate through the vertices and assign indices sequentially
	for (unsigned int i = 0; i < vertices.size(); ++i) {
		indices.push_back(i);
	}
}

/**
 * @brief Retrieves the mesh data parsed by the OBJLoader.
 *
 * This function returns the mesh data parsed by the OBJLoader as a Mesh object.
 * The Mesh object contains information about vertices, texture coordinates, normals, and faces that make up the mesh.
 *
 * @return A Mesh object containing the parsed mesh data.
 */
Mesh OBJLoader::getMesh()
{
	//Hey, i know it's spelled wrong.
	//But I've vertices already defined.
	std::vector<Vertex> vertexes;

	for (unsigned int i = 0; i < vertices.size(); i++) {
		Vertex vertex;
		vertex.Position = vertices[i];
		vertex.UVs = i < uvs.size() ? uvs[i] : glm::vec2{ 0.5, 0.5 };
		vertex.Normal = i < normals.size() ? normals[i] : glm::vec3{ 0, 0, 0 };

		vertexes.push_back(vertex);
	}

	return Mesh(GL_TRIANGLES, vertexes, indices, 0);
}

/**
 * @brief Parses vertex data from the input and stores it as glm::vec3 objects in the output vector.
 *
 * This function parses vertex data from the input vector of strings and stores it as glm::vec3 objects in the output vector. Each string in the input vector represents a vertex in the format "x y z", where x, y, and z are floating-point values representing the x, y, and z coordinates of the vertex.
 *
 * @param input A vector of strings representing the vertex data. Each string should contain three floating-point values separated by whitespace, representing the x, y, and z coordinates of the vertex.
 * @param output An output vector to store the parsed vertex data as glm::vec3 objects.
 */
void OBJLoader::Parse::vertex(std::vector<std::string> input, std::vector<glm::vec3>& output)
{
	glm::vec3 vertex;

	// This could throw errors if not float
	// Try-catch would be a good idea.
	vertex.x = std::stof(input[0]);
	vertex.y = std::stof(input[1]);
	vertex.z = std::stof(input[2]);

	output.push_back(vertex);
}

/**
 * @brief Parses texture coordinate (UV) data from the input and stores it as glm::vec2 objects in the output vector.
 *
 * This function parses texture coordinate (UV) data from the input vector of strings and stores it as glm::vec2 objects in the output vector. Each string in the input vector represents a texture coordinate in the format "u v", where u and v are floating-point values representing the horizontal and vertical components of the texture coordinate (UV).
 *
 * @param input A vector of strings representing the texture coordinate (UV) data. Each string should contain two floating-point values separated by whitespace, representing the horizontal and vertical components of the texture coordinate (UV).
 * @param output An output vector to store the parsed texture coordinate (UV) data as glm::vec2 objects.
 */
void OBJLoader::Parse::uv(std::vector<std::string> input, std::vector<glm::vec2>& output)
{
	glm::vec2 uv;

	// Again, try catch would be nice
	uv.y = std::stof(input[0]);
	uv.x = std::stof(input[1]);

	output.push_back(uv);
}

/**
 * @brief Parses normal data from the input and stores it as glm::vec3 objects in the output vector.
 *
 * This function parses normal data from the input vector of strings and stores it as glm::vec3 objects in the output vector. Each string in the input vector represents a normal in the format "x y z", where x, y, and z are floating-point values representing the components of the normal.
 *
 * @param input A vector of strings representing the normal data. Each string should contain three floating-point values separated by whitespace, representing the x, y, and z components of the normal.
 * @param output An output vector to store the parsed normal data as glm::vec3 objects.
 */
void OBJLoader::Parse::normal(std::vector<std::string> input, std::vector<glm::vec3>& output)
{
	glm::vec3 normal;

	normal.x = std::stof(input[0]);
	normal.y = std::stof(input[1]);
	normal.z = std::stof(input[2]);

	output.push_back(normal);
}

/**
 * @brief Parses a face definition from the input data and extracts vertex indices, texture coordinate indices, and normal indices.
 *
 * This function parses a face definition represented by a vector of strings, extracts the vertex indices, texture coordinate indices, and normal indices, and stores them in separate vectors. Each string in the input vector represents a vertex, texture coordinate, and normal triplet in the format "vertex_index/texture_index/normal_index". In some cases, the triplet does not have to be complete nor be triplet at all. Allowed formats also include vertex_index, vertex_index//norma_index, etc...
 *
 * @param input A vector of strings representing the face data. Each string should contain vertex, texture coordinate, and normal indices separated by slashes (/).
 * @param vertices_i An output vector to store the parsed vertex indices.
 * @param uvs_i An output vector to store the parsed texture coordinate indices.
 * @param normals_i An output vector to store the parsed normal indices.
 */
void OBJLoader::Parse::face(std::vector<std::string> input, std::vector<unsigned int>& vertices_i, std::vector<unsigned int>& uvs_i, std::vector<unsigned int>& normals_i)
{
	int vertexCount = input.size();
	int vertexIndex[4], uvIndex[4], normalIndex[4];

	// Iterate over every vertex of an face
	for (int i = 0; i < vertexCount; ++i) {
		std::vector<std::string> values = split(input[i], '/');

		// For each vertex/uv/normal in face, parse 
		for (int i = 0; i < input.size(); ++i) {
			auto token = input[i];
			auto values = split(token, '/');

			for (int j = 0; j < values.size(); ++j) {
				int number;
				std::istringstream iss(values[j]);

				// String to integer conversion was successfull
				if (iss >> number) {
					// This could be done in a better way.
					if (j == 0) vertexIndex[i] = number;
					if (j == 1) uvIndex[i] = number;
					if (j == 2) normalIndex[i] = number;
				}
			}
		}
	}

	// Handle triangular face
	if (vertexCount == 3) {
		for (int i = 0; i < 3; ++i) {
			vertices_i.push_back(vertexIndex[i]);
			uvs_i.push_back(uvIndex[i]);
			normals_i.push_back(normalIndex[i]);
		}
	}
	// Handle quad face
	// Convert it into two triangles
	else if (vertexCount == 4) {
		for (int i = 0; i < 3; ++i) {
			vertices_i.push_back(vertexIndex[i]);
			uvs_i.push_back(uvIndex[i]);
			normals_i.push_back(normalIndex[i]);
		}
		for (int i = 2; i < 5; ++i) {
			vertices_i.push_back(vertexIndex[i % 4]);
			uvs_i.push_back(uvIndex[i % 4]);
			normals_i.push_back(normalIndex[i % 4]);
		}
	}
	else {
		printf("Error: Face format not supported.\n");
		return;
	}
}

std::vector<Material> OBJLoader::Parse::MTL(std::vector<std::string> input)
{
	std::vector<Material> materials;

	if (input.empty()) {
		Logger::error("Object has 'mtllib' line defined, but no path to MTL file was found.");
		return materials;
	}

	std::string filename = input.front();
	std::ifstream file(filename);

	if (!file.is_open()) {
		Logger::error("Unable to open file " + filename);
		return materials;
	}

	Material currentMaterial;
	std::string line;
	while (std::getline(file, line)) {
		std::istringstream iss(line);
		std::string token;
		iss >> token;

		if (token == "newmtl") {
			if (!currentMaterial.name.empty()) {
				materials.push_back(currentMaterial);
				currentMaterial = Material();
			}
			iss >> currentMaterial.name;
		}
		else if (token == "Ka") {
			iss >> currentMaterial.ambient[0] >> currentMaterial.ambient[1] >> currentMaterial.ambient[2];
		}
		else if (token == "Kd") {
			iss >> currentMaterial.diffuse[0] >> currentMaterial.diffuse[1] >> currentMaterial.diffuse[2];
		}
		else if (token == "Ks") {
			iss >> currentMaterial.specular[0] >> currentMaterial.specular[1] >> currentMaterial.specular[2];
		}
		else if (token == "Ns") {
			iss >> currentMaterial.shininess;
		}
	}
	if (!currentMaterial.name.empty()) {
		materials.push_back(currentMaterial);
	}

	return materials;
}
