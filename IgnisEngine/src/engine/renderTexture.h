#pragma once

#include <GL/glew.h>

class RenderTexture {
public:
    RenderTexture(int width, int height);
    ~RenderTexture();
    void bind() const;
    void unbind() const;
    GLuint frameBuffer, renderBuffer, texture;
    int width, height;
};