#pragma once

#include <GL/glew.h>
#include "asset.h"

class Texture : public Asset {
public:
    Texture(unsigned char* data, int width, int height);
    ~Texture();

    void bind() const;
// private:
    GLuint textureHandle;
    int width, height;
};