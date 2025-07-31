#ifndef MODEL_H
#define MODEL_H

#include <string>
#include <vector>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>


#include "Shader.h"
#include "Mesh.h"


class Model
{
public:
	Model(char* path)
	{
		loadModel(path);
	}

	void Draw(Shader& shader);

	Bounds GetBounds()
	{
		return modelCenter;
	}

private:
	std::vector<Texture> textures_loaded;
	
	// model data
	std::vector<Mesh> meshes;
	std::string directory;

	// used to calculate center of model
	std::vector<glm::vec3> vertPositions;
	Bounds modelCenter;

	void loadModel(std::string path);
	void processNode(aiNode* node, const aiScene *scene);
	Mesh processMesh(aiMesh* mesh, const aiScene* scene);
	std::vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);
	unsigned int TextureFromFile(const char* path, const std::string& directory);

	Material loadMaterial(aiMaterial* mat);
	Bounds CalculateBounds(std::vector<glm::vec3> vertPositions);

};


#endif