#pragma once
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include <sstream>
#include <numeric>

#include "Logger.h"
#include "Vertex.h"
#include "Material.h"
#include "Mesh.h"
#include "OBJLoader.h"
#include "StringUtils.h"

struct MeshContainer {
	std::vector< unsigned int > vertices;
	std::vector< unsigned int > uvs;
	std::vector< unsigned int > normals;
	Material material;
};

OBJLoader::OBJLoader(const std::filesystem::path& modelFilename)
{
	std::vector<glm::vec3> temp_vertices;
	std::vector<glm::vec2> temp_uvs;
	std::vector<glm::vec3> temp_normals;

	std::vector<MeshContainer> parts;
	auto basic = MeshContainer();
	parts.push_back(basic);

	Logger::info("Loading " + modelFilename.string());

	std::ifstream file(modelFilename);
	if (!file.is_open()) {
		Logger::error("File " + modelFilename.string() + " does not exist!");
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
		else if (header == "mtllib") OBJLoader::Parse::MTL(tokens, modelFilename.parent_path().string(), materials);
		// Vertices definition
		else if (header == "v") OBJLoader::Parse::vertex(tokens, temp_vertices);
		// UVs definition
		else if (header == "vt") OBJLoader::Parse::uv(tokens, temp_uvs);
		// Normals definition
		else if (header == "vn") OBJLoader::Parse::normal(tokens, temp_normals);
		// Faces
		else if (header == "f") OBJLoader::Parse::face(tokens, parts.back().vertices, parts.back().uvs, parts.back().normals);
		//TODO
		else if (header == "usemtl") {
			// Here we cope that the material exists
			// TODO: Handle errors here
			auto material = materials[tokens[0]];
			auto test = MeshContainer();
			test.material = material;

			Logger::debug("Creating Material Submesh: " + tokens[0]);

			parts.push_back(test);
		}
	}

	std::ostringstream statistics, mat;

	Logger::info("Parsed model " + modelFilename.string());
	Logger::info("Model Parts: " + std::to_string(parts.size()));

	statistics << "Vertices: " << temp_vertices.size() << ", ";
	statistics << "UVs: " << temp_uvs.size() << ", ";
	statistics << "Normals: " << temp_normals.size();

	for (auto i = materials.begin(); i != materials.end(); ++i) {
		mat << i->first << " ";
	}

	Logger::debug("Model Primitives: \t" + statistics.str());
	Logger::debug("Model Materials: \t" + mat.str());

	for (MeshContainer mesh : parts) {
		if (mesh.vertices.empty()) continue;

		//Hey, i know it's spelled wrong.
		//But I've vertices already defined.
		std::vector<Vertex> vertexes;
		std::vector< GLuint > indices;

		// unroll from indirect to direct vertex specification
		// sometimes not necessary, definitely not optimal
		for (unsigned int u = 0; u < mesh.vertices.size(); u++) {
			Vertex vertex;
			unsigned int vertexIndex = mesh.vertices[u];
			unsigned int uvIndex = mesh.uvs[u];
			unsigned int normalIndex = mesh.normals[u];

			// Getting vertices
			if (vertexIndex - 1 < temp_vertices.size()) {
				glm::vec3 position = temp_vertices[vertexIndex - 1];
				vertex.Position = position;
			}

			// Getting UV information
			if (uvIndex - 1 < temp_uvs.size()) {
				glm::vec2 uv = temp_uvs[uvIndex - 1];
				vertex.UVs = uv;
			}
			else {
				vertex.UVs = glm::vec2{ 0.5, 0.5 };
			}

			// Getting Normals
			if (normalIndex - 1 < temp_normals.size()) {
				glm::vec3 normal = temp_normals[normalIndex - 1];
				vertex.Normal = normal;
			}
			else {
				vertex.Normal = glm::vec3{ 0, 0, 0 };
			}

			vertexes.push_back(vertex);
			indices.push_back(u);
		}

		auto instance = Mesh(GL_TRIANGLES, vertexes, indices, 0);
		instance.material = mesh.material;
		submeshes.push_back(instance);
	}
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
 * This function parses a face definition represented by a vector of strings, extracts the vertex indices, texture coordinate indices, and normal indices, and stores them in separate vectors. Each string in the input vector represents a vertex, texture coordinate, and normal triplet in the format "vertex_index/texture_index/normal_index". In some cases, the triplet does not have to be complete nor be triplet at all. Allowed formats also include, but are not limited to: vertex_index, vertex_index//norma_index, etc...
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

/**
 * @brief Parses material data from the input and appends it to the specified vector of Material objects.
 *
 * This function parses material data from the input vector of strings, representing an path to a MTL file, and appends the parsed materials to the specified vector of Material objects.
 * The input vector are tokens of path to an MTL file, potentionally delimited by whitespace.
 * This function takes only a first element of the input vector into account.
 * The function iterates over each line of the MTL file, parsing the material properties and creating Material objects accordingly.
 * The parsed materials are appended to the output vector.
 *
 * @param input A vector of strings representing path(s) to an MTL file.
 * @param path The path to the directory containing the MTL file.
 * @param output A reference to the vector of Material objects to which the parsed materials will be appended.
 */
void OBJLoader::Parse::MTL(std::vector<std::string> input, std::string path, std::unordered_map<std::string, Material>& materials)
{
	if (input.empty()) {
		Logger::error("Object has 'mtllib' line defined, but no path to MTL file was found.");
		return;
	}

	std::string filename = path + "/" + input.front();
	std::ifstream file(filename);

	if (!file.is_open()) {
		Logger::error("Unable to open file " + filename);
		return;
	}

	Material currentMaterial;
	std::string line;
	while (std::getline(file, line)) {
		std::vector<std::string> tokens = split(line, ' ');

		// If the line is empty for some reason, we skip it.
		// This should not happen, but better be safe than sorry.
		if (tokens.empty()) continue;

		// Get the line header
		std::string header = tokens.front();
		tokens.erase(tokens.begin());

		if (header == "newmtl") {
			if (!currentMaterial.name.empty()) materials[currentMaterial.name] = currentMaterial;
			currentMaterial = Material();
			currentMaterial.name = tokens[0];
		}
		else if (header == "Ka") OBJLoader::Parse::ambient(tokens, currentMaterial);
		else if (header == "Kd") OBJLoader::Parse::diffuse(tokens, currentMaterial);
		else if (header == "Ks") OBJLoader::Parse::specular(tokens, currentMaterial);
		else if (header == "d") OBJLoader::Parse::dissolve(tokens, currentMaterial);
		else if (header == "Ns") currentMaterial.shininess = std::stof(tokens[0]);
		else if (header == "map_Kd") OBJLoader::Parse::texture(tokens, currentMaterial);
	}

	if (!currentMaterial.name.empty()) materials[currentMaterial.name] = currentMaterial;
}

/**
 * @brief Parses ambient color data from the input and assigns it to the specified Material object.
 *
 * This function parses ambient color data from the input vector of strings and assigns it to the specified Material object.
 * The input vector should contain three strings representing the red, green, and blue components of the ambient color, respectively.
 * The ambient color components are converted to floating-point values and assigned to the corresponding elements of the Material's ambient vector.
 *
 * @param input A vector of strings representing the ambient color data. The first string should represent the red component, the second string the green component, and the third string the blue component.
 * @param output A reference to the Material object to which the parsed ambient color data will be assigned.
 */
void OBJLoader::Parse::ambient(std::vector<std::string> input, Material& output)
{
	output.ambient[0] = std::stof(input[0]);
	output.ambient[1] = std::stof(input[1]);
	output.ambient[2] = std::stof(input[2]);
}

/**
 * @brief Parses diffuse color data from the input and assigns it to the specified Material object.
 *
 * This function parses diffuse color data from the input vector of strings and assigns it to the specified Material object.
 * The input vector should contain three strings representing the red, green, and blue components of the diffuse color, respectively.
 * The diffuse color components are converted to floating-point values and assigned to the corresponding elements of the Material's diffuse vector.
 *
 * @param input A vector of strings representing the diffuse color data. The first string should represent the red component, the second string the green component, and the third string the blue component.
 * @param output A reference to the Material object to which the parsed diffuse color data will be assigned.
 */
void OBJLoader::Parse::diffuse(std::vector<std::string> input, Material& output)
{
	output.diffuse[0] = std::stof(input[0]);
	output.diffuse[1] = std::stof(input[1]);
	output.diffuse[2] = std::stof(input[2]);
}

/**
 * @brief Parses specular color data from the input and assigns it to the specified Material object.
 *
 * This function parses specular color data from the input vector of strings and assigns it to the specified Material object.
 * The input vector should contain three strings representing the red, green, and blue components of the specular color, respectively.
 * The specular color components are converted to floating-point values and assigned to the corresponding elements of the Material's specular vector.
 *
 * @param input A vector of strings representing the specular color data. The first string should represent the red component, the second string the green component, and the third string the blue component.
 * @param output A reference to the Material object to which the parsed specular color data will be assigned.
 */
void OBJLoader::Parse::specular(std::vector<std::string> input, Material& output)
{
	output.specular[0] = std::stof(input[0]);
	output.specular[1] = std::stof(input[1]);
	output.specular[2] = std::stof(input[2]);
}

/**
 * @brief Parses dissolve data from the input and assigns it to the specified Material object.
 *
 * This function parses dissolve data from the input vector of strings and assigns it to the specified Material object.
 * MTL files represent transparency with dissolve attribute, which ranges from 0 to 1
 * When dissolve is 1, the material is completely opaque.
 * When dissolve is 0, the material is completely transparent.
 * 
 * The input vector should contain one string representing transparency of the object.
 * The parsed dissolve is converted to floating-point value and assigned to the Material.
 *
 * @param input A vector of strings representing the transparency.
 * @param output A reference to the Material object to which the parsed transparency will be assigned.
 */
void OBJLoader::Parse::dissolve(std::vector<std::string> input, Material& output)
{
	output.transparency = std::stof(input[0]);
}

void OBJLoader::Parse::texture(std::vector<std::string> input, Material& output)
{
	GLuint textureID;
	int width, height, nrChannels;
	auto texture = Texture{};
	auto path = input.back();

	unsigned char* data = stbi_load(path.c_str(), &width, &height, &nrChannels, 0);

	texture.scale = glm::vec3(1.0f);

	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);

	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	texture.id = textureID;
	Logger::info("Created Texture with ID: " + std::to_string(textureID));
	output.texture = texture;

	stbi_image_free(data);
}