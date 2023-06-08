#pragma once

#include <unordered_map>
#include <memory>
#include <string>

typedef unsigned long long GUID;

class Asset {
public:
    GUID guid;
};

class AssetManager {
public:
    static Asset* loadOrGetAsset(GUID guid);
    static void recursivelyRegisterAllAssetsInDirectory(const char* directoryPath);
    // TODO: should I move loadShader and loadTexture into here? Or should they stay in their classes
    // b/c they do have a bit of specific stuff in their constructors

private:
    AssetManager() {} // TODO: prob want to make this singleton so I can control lifetime
    static std::unordered_map<GUID, std::unique_ptr<Asset>> loadedAssets;
    static std::unordered_map<GUID, std::string> registeredAssetFilepaths; // Is this the right way to smart ptr to c string?
};