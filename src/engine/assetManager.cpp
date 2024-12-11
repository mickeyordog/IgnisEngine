#include "assetManager.h"

#include <filesystem>
#include <iostream>
#include <nlohmann/json.hpp>
#include <stb_image.h>
#include <fstream>
#include <sstream>
#include <iostream>
#include <assimp/Importer.hpp>
#include "texture.h"
#include "serializationHelper.h"
#include "animationController.h"


std::unordered_map<IgnisGUID, std::unique_ptr<Asset>> AssetManager::loadedAssets;
std::unordered_map<IgnisGUID, AssetFilepathInfo> AssetManager::registeredAssetMetaFilepaths;

// TODO: need to go through here and everywhere assets loaded to make sure they can handle being passed nullptr if not found
nlohmann::json loadJson(const std::string& filepath)
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
Asset* AssetManager::loadOrGetAsset(const std::string& filepath)
{
    nlohmann::json metaFile = loadJson(filepath + ".meta");
    if (metaFile == nullptr)
        return nullptr;
    return AssetManager::loadOrGetAsset((IgnisGUID)metaFile["guid"]);
}

std::unique_ptr<Asset> AssetManager::loadOrGetAssetCopy(const IgnisGUID guid)
{
    // This is kinda weird, why am I doing this again? Would ideally just be using sharedptrs instead of cloning
    return std::unique_ptr<Asset>(loadOrGetAsset(guid)->clone()); // TODO: what if it's nullptr?
}

Asset* AssetManager::loadAndRegisterAsset(const IgnisGUID guid, const AssetFilepathInfo& info)
{
    Asset* ret;
    if (info.metaExtension == "png" || info.metaExtension == "jpg")
    {
        ret = loadTexture(info.actualFilePath);
    }
    else if (info.metaExtension == "shader")
    {
        // TODO: should make shader an asset that points to 2 shader source assets
        std::string vsFilepath = info.pathWithoutMetaExtension + ".vs";
        std::string fsFilepath = info.pathWithoutMetaExtension + ".fs";
        ret = loadShader(vsFilepath, fsFilepath);
    }
    else if (info.metaExtension == "scene")
    {
        ret = loadScene(info.actualFilePath);
    }
    else if (info.metaExtension == "controller") 
    {
        ret = loadAnimationController(info.actualFilePath);
    }
    else if (info.metaExtension == "clip")
    {
        ret = loadAnimationClip(info.actualFilePath);
    }
    else if (info.metaExtension == "obj")
    {
        ret = loadModel(info.actualFilePath);
    }
    else
    {
        std::cout << info.actualFilePath << " is not a supported file type" << std::endl;
        return nullptr;
    }
    AssetManager::loadedAssets[guid] = std::unique_ptr<Asset>(ret);
    ret->guid = guid;
    return ret;
}

// TODO: this should also add .meta files for all assets that don't have one
void AssetManager::recursivelyRegisterAllAssetsInDirectory(const char* directoryPath)
{
    for (const auto& entry : std::filesystem::directory_iterator(directoryPath)) {
        if (entry.path().extension().compare(".meta") == 0) {
            // TODO: this uses std::string which textbook guy said not to. However it only does it once per game startup so probably fine. But maybe look into removing, I couldn't figure it out
            AssetFilepathInfo info = getFileExtensionInfoFromFilePath(entry.path().string());

            std::ifstream f(entry.path());
            nlohmann::json json = nlohmann::json::parse(f); // Is this alright to do? It probably copies the string yeah?
            registeredAssetMetaFilepaths[json["guid"]] = info; 
        }
        else if (entry.is_directory()) {
            recursivelyRegisterAllAssetsInDirectory(entry.path().string().c_str());
        }
    }
}

Texture* AssetManager::loadTexture(const std::string& filepath)
{
    stbi_set_flip_vertically_on_load(true); // can prob remove this and change flipping back on image part
    int width, height, numChannels;
    unsigned char* data = stbi_load(filepath.c_str(), &width, &height, &numChannels, 0);
    if (data == NULL)
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    Texture* texture = new Texture(data, width, height, numChannels, TextureType::DIFFUSE);
    stbi_image_free(data);

    return texture;
}

Shader* AssetManager::loadShader(const std::string& vsFilepath, const std::string& fsFilepath)
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

Scene* AssetManager::loadScene(const std::string& filepath)
{
    nlohmann::ordered_json sceneJson = loadJson(filepath);
    return SerializationHelper::deserializeScene(sceneJson);
}

AnimationController* AssetManager::loadAnimationController(const std::string& filepath)
{
    nlohmann::ordered_json animControllerJson = loadJson(filepath);
    return SerializationHelper::deserializeAnimationController(animControllerJson);
}

AnimationClip* AssetManager::loadAnimationClip(const std::string& filepath)
{
    nlohmann::ordered_json animClipJson = loadJson(filepath);
    return SerializationHelper::deserializeAnimationClip(animClipJson);
}

Model* AssetManager::loadModel(const std::string& filepath)
{
    Assimp::Importer importer;
    // consider also flags aiProcess_GenNormals and aiProcess_OptimizeMeshes
    const aiScene* scene = importer.ReadFile(filepath, aiProcess_Triangulate | aiProcess_FlipUVs);
    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
        std::cout << "ERROR::ASSIMP::" << importer.GetErrorString() << std::endl;
        return nullptr;
    }

    // TODO: ideally would replace all strings here with filepath types. Can use .stem() and .extension() instead of manual decomposition
    std::filesystem::path path(filepath);
    return new Model(scene, path.parent_path().string());
}

AssetFilepathInfo AssetManager::getFileExtensionInfoFromFilePath(const std::string& filepath)
{
    // Maybe I could store type in file and avoid doing this nonsense
    int lastDotIndex = -1;
    int secondLastDotIndex = -1;
    for (int i = filepath.length() - 1; i >= 0; i--)
    {
        if (filepath[i] == '.')
        {
            if (lastDotIndex == -1)
            {
                lastDotIndex = i;
            }
            else
            {
                secondLastDotIndex = i;
                break;
            }
        }
    }
    if (secondLastDotIndex == -1)
    {
        // TODO: need better way of dealing with errors like this
        std::cout << "Error: file name " << filepath << " has incorrect format" << std::endl;
    }

    std::string pathWithoutMetaExtension = filepath.substr(0, secondLastDotIndex);
    std::string metaExtension = filepath.substr(secondLastDotIndex + 1, lastDotIndex - secondLastDotIndex - 1);
    std::string actualFileExtension = filepath.substr(0, lastDotIndex);
    return { filepath, pathWithoutMetaExtension, metaExtension, actualFileExtension };
}