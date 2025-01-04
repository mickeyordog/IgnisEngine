#include "assetManager.h"

#include <filesystem>
#include <iostream>
#include <nlohmann/json.hpp>
#include <stb_image.h>
#include <fstream>
#include <sstream>
#include <iostream>
#include <assimp/Importer.hpp>
#include <assimp/cimport.h>
#include "texture.h"
#include "serializationHelper.h"
#include "animationController.h"
#include "randomNumberGenerator.h"

// Define these only in *one* .cc file.
#define TINYGLTF_IMPLEMENTATION
//#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
// #define TINYGLTF_NOEXCEPTION // optional. disable exception handling.
#include <tinygltf/tiny_gltf.h>


// TODO: need to go through here and everywhere assets loaded to make sure they can handle being passed nullptr if not found
nlohmann::json loadJson(const std::filesystem::path& filepath)
{
    nlohmann::ordered_json json;
    try
    {
        std::ifstream i(filepath);
        i >> json;
    }
    catch (nlohmann::json::exception e)
    {
        std::cout << "ERROR::JSON filepath: " << filepath << std::endl << e.what() << std::endl;
        return nullptr;
    }
    catch (std::exception e)
    {
        std::cout << "ERROR::JSONLOAD filepath: " << filepath << std::endl << e.what() << std::endl;
        return nullptr;
    }
    return json;
}

// TODO: this will be called by gameobject on each reference to asset it has, so it can get their pointer
// Will probably be AssetManager's job to free all memory at end, or at end of scene
// Can maybe have asset manager load all assets that will be used by the scene when loading the scene itself
// At some point also need to autogenerate meta files, e.g. png meta that lets me slice and specify mip maps and all that
// TODO: do I want to add some type safety here? Currently you need to cast Asset* into e.g. Texture* which might cause crash if unexpected type
Asset* AssetManager::loadOrGetAsset(const IgnisGUID guid)
{
    if (auto search = loadedAssets.find(guid); search != loadedAssets.end())
    {
        return search->second.get(); // Should I just be returning the smart ptr? Maybe use shared ptr instead? To make it more clear who owns lifetime
    }
    else if (auto search = registeredAssetMetaFilepaths.find(guid); search != registeredAssetMetaFilepaths.end()) {
        return loadAndRegisterAsset(guid, search->second);
    }
    std::cout << "Asset with GUID " << guid << " not found" << std::endl;
    return nullptr;
}

// TODO: if loading by path prob shouldn't assume it has a meta file, would also make it easier for me
Asset* AssetManager::loadOrGetAsset(const std::filesystem::path& filepath)
{
    std::filesystem::path metaPath = filepath;
    metaPath.replace_extension(filepath.extension().string() + META_EXT);
    nlohmann::json metaFile = loadJson(metaPath);
    if (metaFile == nullptr)
        return nullptr;
    return AssetManager::loadOrGetAsset((IgnisGUID)metaFile["guid"]);
}

std::unique_ptr<Asset> AssetManager::loadOrGetAssetCopy(const IgnisGUID guid)
{
    // This is kinda weird, why am I doing this again? Would ideally just be using sharedptrs instead of cloning
    return std::unique_ptr<Asset>(loadOrGetAsset(guid)->clone()); // TODO: what if it's nullptr?
}

Asset* AssetManager::loadAndRegisterAsset(const IgnisGUID guid, const std::filesystem::path& filepath)
{
    Asset* ret = nullptr;
    for (const auto& [assetType, extSet] : assetTypeToValidExts) {
        if (extSet.find(filepath.extension().string()) != extSet.end())
        {
            switch (assetType) {
                case AssetType::SHADER:
                {
                    // TODO: should make shader an asset that points to 2 shader source assets with guids
                    std::filesystem::path stem = filepath;
                    stem.replace_extension("");
                    ret = loadShader(stem.string() + ".vs", stem.string() + ".fs");
                    break;
                }
                case AssetType::TEXTURE:
                    ret = loadTexture(filepath);
                    break;
                case AssetType::MODEL:
                    ret = loadModel(filepath);
                    break;
                case AssetType::SCENE:
                    ret = loadScene(filepath);
                    break;
                default:
                    break;
            }
            break;
        }
    }
    if (ret == nullptr) {
        std::cout << "Loaded asset is nullptr, " << filepath << " may not be a supported file type" << std::endl;
        return nullptr;
    }
    AssetManager::loadedAssets[guid] = std::unique_ptr<Asset>(ret);
    ret->guid = guid;
    return ret;
}

void AssetManager::recursivelyRegisterAllAssetsInDirectory(const std::filesystem::path& directoryPath)
{
    std::cout << directoryPath.parent_path();
    for (const auto& entry : std::filesystem::recursive_directory_iterator(directoryPath)) {
        if (entry.is_regular_file()) {
            if (entry.path().extension().compare(META_EXT) == 0)
            {
                std::ifstream f(entry.path());
                nlohmann::json json = nlohmann::json::parse(f);
                IgnisGUID guid = json[GUID_STR];
                if (registeredAssetMetaFilepaths.find(guid) == registeredAssetMetaFilepaths.end()) {
                    // Register path without .meta
                    std::string path = entry.path().string();
                    registeredAssetMetaFilepaths[guid] = path.substr(0, path.length() - strlen(META_EXT));
                }
            } else {
                // check if it's a valid asset extension
                for (const auto& pair : assetTypeToValidExts) {
                    const auto& extSet = pair.second;
                    const std::string ext = entry.path().extension().string();
                    if (extSet.find(ext) != extSet.end()) {
                        // Check if meta file already exists
                        std::filesystem::path metaPath = entry.path();
                        metaPath.replace_extension(ext + META_EXT);
                        if (!std::filesystem::exists(metaPath))
                        {
                            // Create meta file
                            std::ofstream f(metaPath);
                            nlohmann::json json;
                            std::cout << "Creating meta file: " << metaPath << std::endl;
                            // TODO: could check if guid already used?
                            IgnisGUID guid = RandomNumberGenerator::getRandomInteger();
                            json[GUID_STR] = guid;
                            f << std::setw(4) << json << std::endl;
                            registeredAssetMetaFilepaths[guid] = entry.path();
                            // TODO: this doesn't put assets into inspector on first time for some reason
                        }
                        break;
                    }
                }
            }
        }
    }
}

Texture* AssetManager::loadTexture(const std::filesystem::path& filepath)
{
    // Don't flip vertically so we match up with uv coordinates
    // stbi_set_flip_vertically_on_load(true); // can prob remove this and change flipping back on image part
    int width, height, numChannels;
    unsigned char* data = stbi_load(filepath.string().c_str(), &width, &height, &numChannels, 0);
    if (data == NULL)
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    Texture* texture = new Texture(data, width, height, numChannels, TextureType::DIFFUSE);
    stbi_image_free(data);

    return texture;
}

Shader* AssetManager::loadShader(const std::filesystem::path& vsFilepath, const std::filesystem::path& fsFilepath)
{
    std::string vertexCode;
    std::string fragmentCode;
    std::ifstream vShaderFile;
    std::ifstream fShaderFile;

    vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    try
    {
        vShaderFile.open(vsFilepath.c_str());
        fShaderFile.open(fsFilepath.c_str());

        std::stringstream vShaderStream, fShaderStream;
        vShaderStream << vShaderFile.rdbuf();
        fShaderStream << fShaderFile.rdbuf();

        vShaderFile.close();
        fShaderFile.close();

        vertexCode = vShaderStream.str();
        fragmentCode = fShaderStream.str();
    }
    catch (std::ifstream::failure e)
    {
        std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ " << std::endl << e.what() << std::endl;
    }
    const char* vShaderCode = vertexCode.c_str();
    const char* fShaderCode = fragmentCode.c_str();

    return new Shader(vShaderCode, fShaderCode);
}

Scene* AssetManager::loadScene(const std::filesystem::path& filepath)
{
    nlohmann::ordered_json sceneJson = loadJson(filepath);
    return SerializationHelper::deserializeScene(sceneJson);
}

AnimationController* AssetManager::loadAnimationController(const std::filesystem::path& filepath)
{
    nlohmann::ordered_json animControllerJson = loadJson(filepath);
    return SerializationHelper::deserializeAnimationController(animControllerJson);
}

AnimationClip* AssetManager::loadAnimationClip(const std::filesystem::path& filepath)
{
    nlohmann::ordered_json animClipJson = loadJson(filepath);
    return SerializationHelper::deserializeAnimationClip(animClipJson);
}

Model* AssetManager::loadModel(const std::filesystem::path& filepath, bool isBinary)
{
     Assimp::Importer importer;
     const aiScene* scene = importer.ReadFile(filepath.string(), aiProcess_Triangulate | aiProcess_FlipUVs/* | aiProcess_JoinIdenticalVertices | aiProcess_OptimizeMeshes | aiProcess_OptimizeGraph | aiProcess_GenNormals*/);
     if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
     {
         std::cout << "ERROR::ASSIMP::" << importer.GetErrorString() << std::endl;
         return nullptr;
     }
     Model* model = new Model(scene, filepath.parent_path().string());
     //aiReleaseImport(scene); // Seems to crash when I do this? Maybe it gets deleted automatically
     return model;

    //tinygltf::Model model;
    //tinygltf::TinyGLTF loader;
    //std::string err;
    //std::string warn;

    //bool ret;
    //if (isBinary) {
    //    ret = loader.LoadBinaryFromFile(&model, &err, &warn, filepath.string()); // for binary glTF(.glb)
    //} else {
    //    ret = loader.LoadASCIIFromFile(&model, &err, &warn, filepath.string()); // for text gltf
    //}

    //if (!warn.empty()) {
    //    printf("Warn: %s\n", warn.c_str());
    //}

    //if (!err.empty()) {
    //    printf("Err: %s\n", err.c_str());
    //}

    //if (!ret) {
    //    printf("Failed to parse glTF\n");
    //    return nullptr;
    //}

    //std::filesystem::path path(filepath);
    //return new Model(model, path.parent_path().string());
}