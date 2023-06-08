#pragma once

#include <unordered_map>
#include <memory>
#include <string>
#include "texture.h"

typedef unsigned long long IgnisGUID;

class Asset {
public:
    IgnisGUID guid;
};

class AssetManager {
public:
    static Asset* loadOrGetAsset(IgnisGUID guid);
    static void recursivelyRegisterAllAssetsInDirectory(const char* directoryPath);
    static void unloadAsset(IgnisGUID guid) { loadedAssets.erase(guid); }
    static void unloadAllAssets() { loadedAssets.clear(); }
    // TODO: should I move loadShader and loadTexture into here? Or should they stay in their classes
    // b/c they do have a bit of specific stuff in their constructors

private:
    AssetManager() {} // TODO: prob want to make this singleton so I can control lifetime
    Texture* loadTexture(std::string& filepath);
    static std::unordered_map<IgnisGUID, std::unique_ptr<Asset>> loadedAssets;
    static std::unordered_map<IgnisGUID, std::string> registeredAssetFilepaths; // Is this the right way to smart ptr to c string?
};