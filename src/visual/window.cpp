#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include "window.h"

Window::Window(const char* name, int width, int height)
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

	this->window = SDL_CreateWindow(name, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI);
	if(this->window == NULL)
	{
		printf("Window could not be created! SDL Error: %s\n", SDL_GetError());
	}
}

Window::~Window()
{
	// SDL_GL_DeleteContext(gl_context);
	SDL_DestroyWindow(this->window);
	IMG_Quit();
	TTF_Quit();
	SDL_Quit();
}

void Window::swapWindow()
{
	SDL_GL_SwapWindow(this->window);
}

SDL_Window *Window::getWindow()
{
    return this->window;
}