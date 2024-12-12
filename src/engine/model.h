#pragma once

#include <vector>
#include <filesystem>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "shader.h"
#include "mesh.h"

class Model : public Asset {
public:
    Model(const aiScene* assimpScene, const std::filesystem::path& directory);
    // TODO: another constructor where I can just pass in the mesh(es)
    // Maybe would even be cleaner if Model only got the meshes passed in, and 
    // parsing the aiScene was done in assetManager

    void render(Shader& shader);
    virtual ~Model() override;
    virtual Asset* clone() override { return new Model(*this); }
    virtual AssetType getAssetType() const override { return AssetType::MODEL; }

private:
    // model data
    std::vector<Mesh> meshes;
    void processNode(aiNode* node, const aiScene* scene, const std::filesystem::path& directory);
    Mesh processMesh(aiMesh* mesh, const aiScene* scene, const std::filesystem::path& directory);
    std::vector<Texture*> loadMaterialTextures(aiMaterial* mat, aiTextureType type, TextureType textureType, const std::filesystem::path& directory);
};