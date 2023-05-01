#include <SDL.h>
#include "window.h"

Window::Window(const char* name, int width, int height)
{
    this->window = SDL_CreateWindow(name, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_SHOWN);
	if(this->window == NULL)
	{
		printf("Window could not be created! SDL Error: %s\n", SDL_GetError());
	}

    this->renderer = SDL_CreateRenderer(this->window, -1, SDL_RENDERER_ACCELERATED);
    if(this->renderer == NULL)
    {
        printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
    }

    if (SDL_SetRenderDrawColor(this->renderer, 0xFF, 0xFF, 0xFF, 0xFF) < 0)
    {
        printf("Could not set render draw color! SDL Error: %s\n", SDL_GetError());
    }

}

Window::~Window()
{
    SDL_DestroyRenderer(this->renderer);
    SDL_DestroyWindow(this->window);
}

void Window::clearRenderer()
{
    SDL_RenderClear(this->renderer);
}

void Window::presentRenderer()
{
    SDL_RenderPresent(this->renderer);
}

Sprite *Window::loadSprite(const char *path)
{
    return new Sprite(path, this->renderer);
}

void Window::renderSprite(Sprite *sprite, int x, int y, int width, int height)
{
    SDL_Rect renderQuad = {x, y, width, height};
    if(SDL_RenderCopy(this->renderer, sprite->getTexture(), NULL, &renderQuad) < 0)
	{
		printf("Unable to render sprite! SDL Error: %s\n", SDL_GetError());
	}
}