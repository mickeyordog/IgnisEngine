#include <SDL.h>
#include "renderer.h"
#include "sprite.h"

Renderer::Renderer(SDL_Window* window)
{
    this->renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if(this->renderer == NULL)
    {
        printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
    }

    if (SDL_SetRenderDrawColor(this->renderer, 0xFF, 0xFF, 0xFF, 0xFF) < 0)
    {
        printf("Could not set render draw color! SDL Error: %s\n", SDL_GetError());
    }
}

Renderer::~Renderer()
{
    SDL_DestroyRenderer(this->renderer);
}

void Renderer::clear()
{
    SDL_RenderClear(this->renderer);
}

void Renderer::present()
{
    SDL_RenderPresent(this->renderer);
}

void Renderer::renderSprite(Sprite* sprite, int x, int y, int width, int height)
{
    SDL_Rect renderQuad = {x, y, width, height};
    if(SDL_RenderCopy(this->renderer, sprite->getTexture(), NULL, &renderQuad) < 0)
	{
		printf("Unable to render sprite! SDL Error: %s\n", SDL_GetError());
	}
}

SDL_Renderer* Renderer::getRenderer()
{
    return this->renderer;
}