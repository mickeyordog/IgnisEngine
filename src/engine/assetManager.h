#pragma once

#include <unordered_map>
#include <memory>
#include <string>
#include "asset.h"
#include "texture.h"
#include "shader.h"
#include "scene.h"

class AnimationController;
class AnimationClip;

struct AssetFilepathInfo {
    std::string fullFilepath;
    std::string pathWithoutMetaExtension;
    std::string metaExtension;
    std::string actualFilePath;
};

// There is a potential problem if e.g. I have a material asset that some objects use
// Then if the asset file changes, how would I update the objects that use it?
// To solve this, probably just have a button in engine that reloads all assets
// If needed, could make a more elegant solution to only update assets that have changed
class AssetManager {
public:
    static Asset* loadOrGetAsset(IgnisGUID guid);
    static std::unique_ptr<Asset> loadOrGetAssetCopy(IgnisGUID guid);
    static void recursivelyRegisterAllAssetsInDirectory(const char* directoryPath);
    static void unloadAsset(IgnisGUID guid) { loadedAssets.erase(guid); }
    static void unloadAllAssets() { loadedAssets.clear(); }
    static const std::unordered_map<IgnisGUID, AssetFilepathInfo>& getRegisteredAssetMetaFilepaths() { return registeredAssetMetaFilepaths; }

private:
    AssetManager() {} // TODO: prob want to make this singleton so I can control lifetime
    static Asset* loadAndRegisterAsset(IgnisGUID guid, AssetFilepathInfo& info);
    static Texture* loadTexture(std::string& filepath);
    static Shader* loadShader(std::string& filepath, std::string& fsFilepath);
    static Scene* loadScene(std::string& filepath);
    static AnimationController* loadAnimationController(std::string& filepath);
    static AnimationClip* loadAnimationClip(std::string& filepath);
    static AssetFilepathInfo getFileExtensionInfoFromFilePath(std::string filepath);
    static std::unordered_map<IgnisGUID, std::unique_ptr<Asset>> loadedAssets;
    static std::unordered_map<IgnisGUID, AssetFilepathInfo> registeredAssetMetaFilepaths; // Is this the right way to smart ptr to c string?
    // TODO: refactor into map from guid to AssetFilepathInfo
};