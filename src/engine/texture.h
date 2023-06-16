#pragma once

#include <GL/glew.h>
#include "asset.h"

class Texture : public Asset {
public:
    Texture(unsigned char* data, int width, int height);
    virtual ~Texture() override;
    virtual Asset* clone() override { return new Texture(*this); }

    void bind() const;
// private:
    GLuint textureHandle;
    int width, height;
};