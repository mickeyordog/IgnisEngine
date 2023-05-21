#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include "SDLContext.h"

SDLContext::SDLContext(const char* name, int width, int height)
{
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		printf("SDL could not initialize! SDL Error: %s\n", SDL_GetError());
	}
	int imgFlags = IMG_INIT_PNG;
	if (!(IMG_Init(imgFlags) & imgFlags))
	{
		printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
	}
	if (TTF_Init() == -1)
	{
		printf("SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError());
	}

	this->window = SDL_CreateWindow(name, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
	if(this->window == NULL)
	{
		printf("Window could not be created! SDL Error: %s\n", SDL_GetError());
	}
}

SDLContext::~SDLContext()
{
	// SDL_GL_DeleteContext(gl_context);
	SDL_DestroyWindow(this->window);
	IMG_Quit();
	TTF_Quit();
	SDL_Quit();
}

void SDLContext::swapWindow()
{
	SDL_GL_SwapWindow(this->window);
}

SDL_Window *SDLContext::getWindow()
{
    return this->window;
}