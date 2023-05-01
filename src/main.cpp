#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <math.h>
#include "window.h"
#include "gameObject.h"


void close();

bool init()
{
	bool success = true;

	if( SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		printf("SDL could not initialize! SDL Error: %s\n", SDL_GetError());
		success = false;
	}
	int imgFlags = IMG_INIT_PNG;
	if(!(IMG_Init(imgFlags) & imgFlags))
	{
		printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
		success = false;
	}

	return success;
}



void close()
{
	IMG_Quit();
	SDL_Quit();
}


int main( int argc, char* args[] )
{
	if(!init())
	{
		printf("Failed to initialize!\n");
		return -1;
	}

	Window window("Ignis Engine", 640, 480);
	Sprite* sprite = window.loadSprite("../assets/texture.png");
	GameObject gameObject = GameObject(sprite);

	bool quit = false;
	SDL_Event e;

	while(!quit)
	{
		while(SDL_PollEvent(&e) != 0)
		{
			if( e.type == SDL_QUIT )
			{
				quit = true;
			}
		}

		window.clearRenderer();

		window.renderSprite(sprite, 100 + 100 * cos(SDL_GetTicks() / 1000.0), 100 + 100 * sin(SDL_GetTicks() / 1000.0), 640/2, 480/2);
		window.presentRenderer();
	}

	delete(sprite);
	close();

	return 0;
}