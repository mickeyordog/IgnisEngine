#include "model.h"
#include <iostream>
#include "assetManager.h"

Model::Model(const aiScene* assimpScene, const std::filesystem::path& directory)
{
    processNode(assimpScene->mRootNode, assimpScene, directory);
}

// Not working, try with a simpler mesh like just one triangle
Mesh Model::processMesh(const tinygltf::Model& model, const tinygltf::Mesh& mesh) {
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<Texture*> textures;
    for (size_t i = 0; i < mesh.primitives.size(); ++i) {
        const tinygltf::Primitive& primitive = mesh.primitives[i];

        // indices
        {
            const tinygltf::Accessor& indexAccessor = model.accessors[primitive.indices];
            const tinygltf::BufferView& indexBufferView = model.bufferViews[indexAccessor.bufferView];
            const tinygltf::Buffer& indexBuffer = model.buffers[indexBufferView.buffer];
            int start = indexBufferView.byteOffset + indexAccessor.byteOffset;
            int end = start + indexBufferView.byteLength;
            for (int i = start; i < end; i += 2) { // +2 because unsigned short, use byteStride instead
                unsigned short* index = (unsigned short*)&indexBuffer.data[i];
                indices.push_back(*index);
            }
        }

        // vertices
        std::vector<Vec3> positions;
        std::vector<Vec3> normals;
        std::vector<Vec2> texCoords;
        for (const auto& attrib : primitive.attributes) {
            // maybe also want to be checking that data is correct type
            const tinygltf::Accessor& accessor = model.accessors[attrib.second];
            const tinygltf::BufferView& bufferView = model.bufferViews[accessor.bufferView];
            const tinygltf::Buffer& buffer = model.buffers[bufferView.buffer];
            int start = bufferView.byteOffset + accessor.byteOffset;
            int end = start + bufferView.byteLength;
            for (int i = start; i < end;) {
                if (attrib.first.compare("POSITION") == 0) {
                    Vec3* vec = (Vec3*)&buffer.data[i];
                    positions.push_back(*vec);
                    i += sizeof(Vec3);
                } else if (attrib.first.compare("NORMAL") == 0) {
                    Vec3* vec = (Vec3*)&buffer.data[i];
                    normals.push_back(*vec);
                    i += sizeof(Vec3);
                } else if (attrib.first.compare("TEXCOORD_0") == 0) {
                    Vec2* vec = (Vec2*)&buffer.data[i];
                    texCoords.push_back(*vec);
                    i += sizeof(Vec2);
                }
            }
        }
        for (size_t i = 0; i < positions.size(); ++i) {
            Vertex vertex;
            vertex.Position = positions[i];
            vertex.Normal = normals[i];
            vertex.TexCoords = texCoords[i];
            vertices.push_back(vertex);
        }

        // textures
        for (const tinygltf::Texture& tex : model.textures) {
            if (tex.source > -1) {
                const tinygltf::Image& image = model.images[tex.source];
                // don't like this new here, would be better if texture was external file
                // text version seems to use external texture, not sure if binary version can do that   
                textures.push_back(new Texture(image.image.data(), image.width, image.height, image.component, TextureType::DIFFUSE));
            }
        }
    }
    return Mesh(vertices, indices, textures);
}

void Model::processNode(const tinygltf::Model& model, const tinygltf::Node& node) {
    if ((node.mesh >= 0) && (node.mesh < model.meshes.size())) {
        meshes.push_back(processMesh(model, model.meshes[node.mesh]));
    }
    for (size_t i = 0; i < node.children.size(); i++) {
        processNode(model, model.nodes[node.children[i]]);
    }
}

Model::Model(const tinygltf::Model& model, const std::filesystem::path& directory) { 
    const tinygltf::Scene& scene = model.scenes[model.defaultScene];
    for (size_t i = 0; i < scene.nodes.size(); ++i) {
        processNode(model, model.nodes[scene.nodes[i]]);
    }
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
