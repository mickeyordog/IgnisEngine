#include <SDL_image.h>
#include "sprite.h"

// TODO: not a fan of how this needs a low level object passed to it
//       don't like how Window::loadSprite() has to allocate memory
Sprite::Sprite(char* path, SDL_Renderer* initRenderer)
{
    this->texture = IMG_LoadTexture(initRenderer, path);
    if(this->texture == NULL)
	{
		printf("Unable to load texture %s! SDL_image Error: %s\n", path, IMG_GetError());
	}
}

Sprite::~Sprite()
{
    SDL_DestroyTexture(this->texture);
}

SDL_Texture *Sprite::getTexture()
{
    return this->texture;
}
