#include "model.h"
#include <iostream>
#include "assetManager.h"

Model::Model(const aiScene* assimpScene, const std::filesystem::path& directory)
{
    // double factor(0.0);
    // if (assimpScene->mMetaData->Get("UnitScaleFactor", factor)) {
    //     std::cout << "Unit scale factor is " << factor << std::endl;
    // }
    // else {
    //     std::cout << "Failed to get unit scale factor!" << std::endl;
    // }
    processNode(assimpScene->mRootNode, assimpScene, directory);
}

Model::~Model()
{
    // TODO
}

void Model::render(Shader& shader)
{ 
    for (auto& mesh : meshes)
    {
        mesh.render(shader);
    }
}

void Model::processNode(aiNode* node, const aiScene* scene, const std::filesystem::path& directory)
{ 
    // process all the node’s meshes (if any)
    for (unsigned int i = 0; i < node->mNumMeshes; i++)
    {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        meshes.push_back(processMesh(mesh, scene, directory));
    }

    // then do the same for each of its children
    for (unsigned int i = 0; i < node->mNumChildren; i++)
    {
        processNode(node->mChildren[i], scene, directory);
    }
}

Mesh Model::processMesh(aiMesh* mesh, const aiScene* scene, const std::filesystem::path& directory)
{
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<Texture*> textures;
    // process vertex positions, normals and texture coordinates
    for (unsigned int i = 0; i < mesh->mNumVertices; i++)
    {
        Vertex vertex;
        vertex.Position = { mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z };
        // std::cout << vertex.Position << std::endl;
        vertex.Normal = { mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z };
        if (mesh->mTextureCoords[0]) // does the mesh contain texture coordinates?
        {
            vertex.TexCoords = { mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y };
        }
        else
        {
            std::cout << "Model does not contain texture coordinates!" << std::endl;
            vertex.TexCoords = { 0.0f, 0.0f };
        }
        vertices.push_back(vertex);
    }
    // process indices
    for (unsigned int i = 0; i < mesh->mNumFaces; i++)
    {
        const aiFace& face = mesh->mFaces[i];
        for (unsigned int j = 0; j < face.mNumIndices; j++)
            indices.push_back(face.mIndices[j]);
    }
    // process material
    if (mesh->mMaterialIndex >= 0)
    {
        aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
        std::vector<Texture*> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, TextureType::DIFFUSE, directory);
        textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
        std::vector<Texture*> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, TextureType::SPECULAR, directory);
        textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
    }
    return Mesh(vertices, indices, textures);
}

std::vector<Texture*> Model::loadMaterialTextures(aiMaterial* mat, aiTextureType type, TextureType textureType, const std::filesystem::path& directory)
{
    std::vector<Texture*> textures;
    for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
    {
        // Would be nice if this was the full path so I didn't need to carry around the directory
        aiString path;
        mat->GetTexture(type, i, &path);
        std::string filepathStr = path.C_Str();
        // path can return with backslashes, replace them with forward slashes so it works on other platforms
        std::replace(filepathStr.begin(), filepathStr.end(), '\\', '/');
        std::filesystem::path filepath(filepathStr);
        filepath.make_preferred();
        Texture* texture = (Texture*)AssetManager::loadOrGetAsset(directory / filepath);

        textures.push_back(texture);
    }
    return textures;
}
