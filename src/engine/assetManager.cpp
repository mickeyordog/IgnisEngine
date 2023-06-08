#include "assetManager.h"

#include <filesystem>
#include <iostream>

std::unordered_map<GUID, std::unique_ptr<Asset>> AssetManager::loadedAssets;
std::unordered_map<GUID, std::string> AssetManager::registeredAssetFilepaths;

// TODO: this will be called by gameobject on each reference to asset it has, so it can get their pointer
// Will probably be AssetManager's job to free all memory at end, or at end of scene
// Can maybe have asset manager load all assets that will be used by the scene when loading the scene itself
// Probably upon engine load I first want to build a map of all available file assets to their file locations,
// that way I can easily load each asset by guid when I need to without needing to search entire file structure
// At some point also need to autogenerate meta files, e.g. png meta that lets me slice and specify mip maps and all that
Asset* AssetManager::loadOrGetAsset(GUID guid)
{
    if (auto search = loadedAssets.find(guid); search != loadedAssets.end())
    {
        return search->second.get(); // Should I just be returning the smart ptr? Maybe use shared ptr instead? To make it more clear who owns lifetime
    }
    // TODO: need to load asset
    // I guess go through asset files, find the one with the matching GUID, and load it
    // The file will also tell me what type it'll be, then can call corresponding
    // AssetManager::load{AssetType} function
    return nullptr;
}

void AssetManager::recursivelyRegisterAllAssetsInDirectory(const char* directoryPath)
{
    for (const auto& entry : std::filesystem::directory_iterator(directoryPath)) {
        // std::cout << entry.path() << std::endl;
        if (entry.path().extension().compare(".meta") == 0) {
            // TODO: this uses std::string which textbook guy said not to. However it only does it once per game startup so probably fine. But maybe look into removing, I couldn't figure it out
            registeredAssetFilepaths[0] = entry.path().string(); // Is this alright to do? It probably copies the string yeah?
            std::cout << "Found meta called " << entry.path().filename() << std::endl;
            // TODO: read GUID from meta file json
        }
        else if (entry.is_directory()) {
            recursivelyRegisterAllAssetsInDirectory(entry.path().string().c_str());
        }
    }
}
