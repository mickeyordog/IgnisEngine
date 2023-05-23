#pragma once

#include "GL/glew.h"

class Texture {
public:
    Texture(const char* filePath);
    ~Texture();

    void bind() const;
// private:
    GLuint textureHandle;
    int width, height;
};