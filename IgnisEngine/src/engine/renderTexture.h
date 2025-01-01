#pragma once

#include <GL/glew.h>

class RenderTexture {
public:
    RenderTexture(int width, int height);
    ~RenderTexture();
    void bind();
    void unbind();
    GLuint frameBuffer, renderBuffer, texture;
    int width, height;
};