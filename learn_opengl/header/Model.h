#ifndef MODEL_H
#define MODEL_H

#include <glad/glad.h> 

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "../header/stb_image.h"
#include <../include/assimp/Importer.hpp>
#include <../include/assimp/scene.h>
#include <../include/assimp/postprocess.h>

#include "../header/Shader.h"
#include "../header/Mesh.h"

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <map>
#include <vector>

unsigned int TextureFromFile(const char* path, const std::string& directory, bool gamma = false);

class Model
{
public:
	Model(const char* path);
	void draw(Shader &shader);

private:
	// Model data
	std::vector<Mesh> m_meshes;
	// directory containing the model file
	std::string m_directory;
	// all the loaded textures (to check if one has already been loaded before, in which case it is not necessary to reload it)
	std::vector<Texture> m_textures_loaded;

	void loadModel(std::string path);
	void processNode(aiNode* node, const aiScene* scene);
	Mesh processMesh(aiMesh* mesh, const aiScene* scene);
	std::vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);
};

#endif
