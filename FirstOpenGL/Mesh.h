#ifndef MESH_H
#define MESH_H

// system headers then program headers
#include <string>
#include <vector>
#include <glad/glad.h> 
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Shader.h"

struct Vertex
{
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2 TexCoords;
};

struct Texture
{
	unsigned int id;
	std::string type;
	std::string path; // store path to compare with other textures
};

struct Material
{
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;
	float shininess;
	float opacity;

	// Constructor with default values
	Material()
		: ambient(1.0f, 1.0f, 0.2f)
		, diffuse(1.0f, 1.0f, 1.0f)
		, specular(1.0f, 1.0f, 1.0f)
		, shininess(32.0f)
		, opacity(1.0f)
	{}
};

// needed for effect
struct Bounds {
	glm::vec3 center;
	/// <summary>
	///  distance from model center to its furthest point
	/// </summary>
	float radius;
};

class Mesh
{
	public:
		// mesh data
		std::vector<Vertex> vertices;
		std::vector<unsigned int> indices;
		std::vector<Texture> textures;
		Material material;

		Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures, Material material);
		void Draw(Shader& shader);

	private:
		// render data
		unsigned int VAO, VBO, EBO;

		void setupMesh();






};


#endif

