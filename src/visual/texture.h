#pragma once

class Texture {
public:
    Texture(const char* filePath);
    ~Texture();
private:
    GLuint textureHandle;
};