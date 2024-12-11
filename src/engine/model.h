#pragma once

#include <vector>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "shader.h"
#include "mesh.h"

class Model : public Asset {
public:
    Model(const aiScene* assimpScene);

    void render(Shader& shader);
    virtual ~Model() override;
    virtual Asset* clone() override { return new Model(*this); }

private:
    // model data
    std::vector<Mesh> meshes;
    std::string directory;
    void processNode(aiNode* node, const aiScene* scene);
    Mesh processMesh(aiMesh* mesh, const aiScene* scene);
    std::vector<Texture*> loadMaterialTextures(aiMaterial* mat, aiTextureType type, TextureType textureType);
};