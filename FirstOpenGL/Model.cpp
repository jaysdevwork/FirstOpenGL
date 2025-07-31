#include "Model.h"
#include "stb_image.h"

void Model::Draw(Shader& shader)
{
	for (unsigned int i = 0; i < meshes.size(); i++)
	{
		meshes[i].Draw(shader);
	}
}

void Model::loadModel(std::string path)
{
	Assimp::Importer import;
	const aiScene *scene = import.ReadFile(path, aiProcess_Triangulate); // | aiProcess_FlipUVs to flip em

	// check if scene and root node are not null and ensure flags do not indicate incomplete data
	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		std::cout << "ERROR::ASSIMP::" << import.GetErrorString() << std::endl;
		return;
	}
	directory = path.substr(0, path.find_last_of('/'));
	processNode(scene->mRootNode, scene);

	// Calculate bounds for model once all verts have been collected in processing
	this->modelCenter = CalculateBounds(this->vertPositions);
}

void Model::processNode(aiNode* node, const aiScene* scene)
{
	// process all the node's meshes (if any)
	// not directly looping through scene's meshes bc this method makes easier
	// to create parent-child relations with meshes later on
	for (unsigned int i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		meshes.push_back(processMesh(mesh, scene));
	}
	// then do the same for each of its children
	for (unsigned int i = 0; i < node->mNumChildren; i++)
	{
		processNode(node->mChildren[i], scene);
	}

}

Mesh Model::processMesh(aiMesh* mesh, const aiScene* scene)
{
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<Texture> textures;
	Material material;

	// Process vertices
	for (unsigned int i = 0; i < mesh->mNumVertices; i++)
	{
		Vertex vertex;

		// Process vertex positions
		glm::vec3 vector;
		vector.x = mesh->mVertices[i].x;
		vector.y = mesh->mVertices[i].y;
		vector.z = mesh->mVertices[i].z;
		vertex.Position = vector;

		// Process normals
		if (mesh->mNormals)
		{
			vector.x = mesh->mNormals[i].x;
			vector.y = mesh->mNormals[i].y;
			vector.z = mesh->mNormals[i].z;
			vertex.Normal = vector;
		}

		// Process texture coordinates
		if (mesh->mTextureCoords[0])
		{
			glm::vec2 vec;
			vec.x = mesh->mTextureCoords[0][i].x;
			vec.y = mesh->mTextureCoords[0][i].y;
			vertex.TexCoords = vec;
		}
		else
		{
			vertex.TexCoords = glm::vec2(0.0f, 0.0f);
		}

		// Collect mesh vertex positions for center calculation
		this->vertPositions.push_back(vertex.Position); // this not necessary but makes clear we referring to member variable

		vertices.push_back(vertex);
	}

	// Process indices (MOVED OUTSIDE the vertices loop)
	for (unsigned int i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];
		for (unsigned int j = 0; j < face.mNumIndices; j++)
		{
			indices.push_back(face.mIndices[j]);
		}
	}

	// Process material (MOVED OUTSIDE the vertices loop)
	if (mesh->mMaterialIndex >= 0)
	{
		aiMaterial* material_as = scene->mMaterials[mesh->mMaterialIndex];

		std::vector<Texture> diffuseMaps = loadMaterialTextures(material_as, aiTextureType_DIFFUSE, "texture_diffuse");
		textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

		std::vector<Texture> specularMaps = loadMaterialTextures(material_as, aiTextureType_SPECULAR, "texture_specular");
		textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());

		material = loadMaterial(material_as);
	}

	// Return the mesh (MOVED OUTSIDE all loops)
	return Mesh(vertices, indices, textures, material);
}

std::vector<Texture> Model::loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName)
{
	// compare texture path with all textures already loaded, skipping those that are
	std::vector<Texture> textures;
	for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
	{
		aiString str;
		mat->GetTexture(type, i, &str);
		bool skip = false;
		for (unsigned int j = 0; j < textures_loaded.size(); j++)
		{
			if (std::strcmp(textures_loaded[j].path.data(), str.C_Str()) == 0)

			{
				textures.push_back(textures_loaded[j]);
				skip = true;
				break;
			}
		}
		// if texture hasnt been loaded already, load it
		if (!skip)
		{
			Texture texture;
			texture.id = TextureFromFile(str.C_Str(), directory);
			texture.type = typeName;
			texture.path = str.C_Str();
			textures.push_back(texture);
			textures_loaded.push_back(texture); // add to loaded textures
		}
	} 
	return textures;
}

unsigned int Model::TextureFromFile(const char* path, const std::string& directory)
{
	std::string filename = std::string(path);
	filename = directory + '/' + filename;

	unsigned int textureID;
	glGenTextures(1, &textureID);

	int width, height, nrComponents;
	unsigned char* data = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);
	if (data)
	{
		GLenum format;
		if (nrComponents == 1)
		{
			format = GL_RED;
		} 
		else if (nrComponents == 3)
		{
			format = GL_RGB;
		}
		else if (nrComponents == 4)
		{
			format = GL_RGBA;
		}

		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		stbi_image_free(data);
	}
	else
	{
		std::cout << "Texture failed to load at path: " << path << std::endl;
		stbi_image_free(data);
	}
	return textureID;
}

Material Model::loadMaterial(aiMaterial* mat)
{
	Material material; // Already has default values from constructor
	aiColor3D color;
	float value;

	// Get diffuse color (Kd in MTL)
	if (mat->Get(AI_MATKEY_COLOR_DIFFUSE, color) == AI_SUCCESS)
	{
		material.diffuse = glm::vec3(color.r, color.g, color.b);
	}

	// Get ambient color (Ka in MTL)
	if (mat->Get(AI_MATKEY_COLOR_AMBIENT, color) == AI_SUCCESS)
	{
		material.ambient = glm::vec3(color.r, color.g, color.b);
	}

	// Get specular color (Ks in MTL)
	if (mat->Get(AI_MATKEY_COLOR_SPECULAR, color) == AI_SUCCESS)
	{
		material.specular = glm::vec3(color.r, color.g, color.b);
	}

	// Get shininess (Ns in MTL)
	if (mat->Get(AI_MATKEY_SHININESS, value) == AI_SUCCESS)
	{
		material.shininess = value;
	}

	// Get transparency/dissolve (d in MTL)
	if (mat->Get(AI_MATKEY_OPACITY, value) == AI_SUCCESS)
	{
		material.opacity = value;
	}

	return material;
}


Bounds Model::CalculateBounds(std::vector<glm::vec3> vertPositions)
{
	if (vertPositions.empty()) {
		return { glm::vec3(0.0f), 0.0f };
	}

	// Initialize min/max bounds with first vertex
	glm::vec3 minBounds = vertPositions[0];
	glm::vec3 maxBounds = vertPositions[0];

	// Loop through all vertices to find min/max bounds
	for (const auto& vertex : vertPositions) {
		minBounds.x = std::min(minBounds.x, vertex.x);
		minBounds.y = std::min(minBounds.y, vertex.y);
		minBounds.z = std::min(minBounds.z, vertex.z);

		maxBounds.x = std::max(maxBounds.x, vertex.x);
		maxBounds.y = std::max(maxBounds.y, vertex.y);
		maxBounds.z = std::max(maxBounds.z, vertex.z);
	}

	// Calculate center as average of min/max bounds
	glm::vec3 center = (minBounds + maxBounds) * 0.5f;

	// Calculate radius as distance from center to farthest corner of bounding box
	float radius = glm::length(maxBounds - center);

	return { center, radius };
}
