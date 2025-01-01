#pragma once

#include <unordered_map>
#include <memory>
#include <string>
#include "asset.h"
#include "texture.h"
#include "shader.h"
#include "scene.h"
#include "model.h"

class AnimationController;
class AnimationClip;

// There is a potential problem if e.g. I have a material asset that some objects use
// Then if the asset file changes, how would I update the objects that use it?
// To solve this, probably just have a button in engine that reloads all assets
// If needed, could make a more elegant solution to only update assets that have changed
class AssetManager {
public:
    static Asset* loadOrGetAsset(IgnisGUID guid);
    static Asset* loadOrGetAsset(const std::filesystem::path& filepath);
    static std::unique_ptr<Asset> loadOrGetAssetCopy(const IgnisGUID guid);
    static void recursivelyRegisterAllAssetsInDirectory(const std::filesystem::path& directoryPath);
    static void unloadAsset(const IgnisGUID guid) { loadedAssets.erase(guid); }
    static void unloadAllAssets() { loadedAssets.clear(); }
    static const std::unordered_map<IgnisGUID, std::filesystem::path>& getRegisteredAssetMetaFilepaths() { return registeredAssetMetaFilepaths; }

private:
    AssetManager() {} // TODO: prob want to make this singleton so I can control lifetime. Otherwise delete ctor
    static Asset* loadAndRegisterAsset(const IgnisGUID guid, const std::filesystem::path& filepath);
    static Texture* loadTexture(const std::filesystem::path& filepath);
    static Shader* loadShader(const std::filesystem::path& vsFilepath, const std::filesystem::path& fsFilepath);
    static Scene* loadScene(const std::filesystem::path& filepath);
    static AnimationController* loadAnimationController(const std::filesystem::path& filepath);
    static AnimationClip* loadAnimationClip(const std::filesystem::path& filepath);
    static Model* loadModel(const std::filesystem::path& filepath, bool isBinary = true);

    inline static std::unordered_map<IgnisGUID, std::unique_ptr<Asset>> loadedAssets;
    inline static std::unordered_map<IgnisGUID, std::filesystem::path> registeredAssetMetaFilepaths; // Is this the right way to smart ptr to c string?
    inline static const std::unordered_map<AssetType, std::unordered_set<std::string>> assetTypeToValidExts {
        {AssetType::SHADER, {".shader"}},
        {AssetType::VERT_SHADER, {".vs"}},
        {AssetType::FRAG_SHADER, {".fs"}},
        {AssetType::TEXTURE, {".png"}},
        {AssetType::MODEL, {".glb", ".gltf"}},
        {AssetType::SCENE, {".scene"}},
    };

    inline static const char* GUID_STR = "guid";
    inline static const char* META_EXT = ".meta";

};