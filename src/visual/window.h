#pragma once

#include <SDL.h>
#include "sprite.h"

class Window {
public:
    Window(char* name, int width, int height);
    ~Window();
    void clearRenderer();
    void presentRenderer();
    Sprite* loadSprite(char* path);
    void renderSprite(Sprite* sprite, int x, int y, int width, int height);

private:
    SDL_Window* window;
    SDL_Renderer* renderer;
};