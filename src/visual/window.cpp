#include <SDL.h>
#include "window.h"

Window::Window(const char* name, int width, int height)
{
	this->window = SDL_CreateWindow(name, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL);
	if(this->window == NULL)
	{
		printf("Window could not be created! SDL Error: %s\n", SDL_GetError());
	}
}

Window::~Window()
{
    SDL_DestroyWindow(this->window);
}

SDL_Window* Window::getWindow()
{
    return this->window;
}