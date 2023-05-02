#pragma once

#include <SDL.h>
#include <SDL_ttf.h>
#include "renderer.h"

class Text {
public:
    Text(const char* text, TTF_Font* font, Renderer* renderer);
    ~Text();
    void render(Renderer* renderer);

private:
    SDL_Texture* texture;
};