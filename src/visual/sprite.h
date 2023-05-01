#pragma once

#include <SDL.h>

class Sprite {
public:
    Sprite(const char* path, SDL_Renderer* renderer);
    ~Sprite();
    SDL_Texture* getTexture();
private:
    SDL_Texture* texture;
};