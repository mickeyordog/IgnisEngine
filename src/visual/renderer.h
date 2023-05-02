#pragma once

#include <SDL.h>
#include "sprite.h"

class Renderer {
public:
    Renderer(SDL_Window* window);
    ~Renderer();
    void clear();
    void present();
    void renderSprite(Sprite* sprite, int x, int y, int width, int height);

    SDL_Renderer* getRenderer();
private:
    SDL_Renderer* renderer;
};