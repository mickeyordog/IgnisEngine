// #include <stdio.h>
#include <iostream>
#include <GL/glew.h>
#include "texture.h"
#include "stb_image.h"

Texture::Texture(const char *filePath)
{
    stbi_set_flip_vertically_on_load(true);
    int width, height, nrChannels;
    unsigned char *data = stbi_load(filePath, &width, &height, &nrChannels, 0); // Why did I have to set 3, but it thinks there are 4 channels in penguin?
    if (data == NULL)
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    std::cout << "Loaded texture " << filePath << " with dimensions " << width << "x" << height << " channels: " << nrChannels << std::endl;
    this->width = width;
    this->height = height;

    glGenTextures(1, &this->textureHandle);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, this->textureHandle);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    stbi_image_free(data);
}

Texture::~Texture()
{
    glDeleteTextures(1, &this->textureHandle);
}

void Texture::bind() const
{
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, this->textureHandle);
}
