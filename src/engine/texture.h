#pragma once

#include <GL/glew.h>
#include "asset.h"

enum class TextureType {
    DIFFUSE,
    SPECULAR,
    NORMAL,
    BUMP
};

class Texture : public Asset {
public:
    Texture(unsigned char* data, int width, int height, int numChannels, TextureType type);
    virtual ~Texture() override;
    virtual Asset* clone() override { return new Texture(*this); }
    virtual AssetType getAssetType() const override { return AssetType::TEXTURE; }

    void bind() const;
    TextureType type;
// private:
    GLuint textureHandle;
    int width, height;
};