#pragma once

class Texture {
public:
    Texture(const char* filePath);
    ~Texture();

    void bind() const;
private:
    GLuint textureHandle;
};