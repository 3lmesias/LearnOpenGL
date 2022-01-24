#pragma once
#include "Mesh.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "stb_image.h"
#ifndef MODEL
#define MODEL

class Model {
public:
	Model(const char* path) {
		LoadModel(path);
	}
	void Draw(Shader2 &shader);
private:
	std::vector<Texture> textures_loaded;
	std::vector<Mesh> meshes;
	std::string  directory;

	void LoadModel(std::string path);
	void ProcessNode(aiNode* node, const aiScene *scene);
	Mesh ProcessMesh(aiMesh* mesh, const aiScene* scene);
	std::vector<Texture> LoadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);

};
#endif // !MODEL
