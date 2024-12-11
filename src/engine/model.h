#pragma once

#include <vector>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "shader.h"
#include "mesh.h"

class Model : public Asset {
public:
    // don't like having to pass around directory like this
    Model(const aiScene* assimpScene, const std::string& directory);

    void render(Shader& shader);
    virtual ~Model() override;
    virtual Asset* clone() override { return new Model(*this); }

private:
    // model data
    std::vector<Mesh> meshes;
    void processNode(aiNode* node, const aiScene* scene, const std::string& directory);
    Mesh processMesh(aiMesh* mesh, const aiScene* scene, const std::string& directory);
    std::vector<Texture*> loadMaterialTextures(aiMaterial* mat, aiTextureType type, TextureType textureType, const std::string& directory);
};