#pragma once

typedef unsigned long long IgnisGUID;
typedef unsigned long long FileID;

// int version of this would need to be stored in meta file, then when registering all assets in directory, could map guid to struct of filepath AND type
// then in editor when I want to see all available e.g. textures to put in sprite renderer, I can iterate over registry and check type
// OTHER OPTION is that I could just check file extension
enum class AssetType { // TODO: not positive I want to do it like this, don't think Unity stores this, and will need to do this manually for each new type
    SHADER,
    MATERIAL,
    TEXTURE,
    MODEL,
    SCENE,
    PREFAB,
    UNKNOWN
};

class Asset {
public:
    virtual ~Asset() = default;
    virtual Asset* clone() = 0;
    virtual AssetType getAssetType() const = 0;
    IgnisGUID guid;
};