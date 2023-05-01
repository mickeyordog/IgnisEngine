#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <math.h>
#include "window.h"
#include "gameObject.h"
#include "timer.h"


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

	Window window("Engine", 640, 480);
	Sprite* sprite = window.loadSprite("../assets/texture.png");
	GameObject gameObject = GameObject(sprite, 100, 100, 640/2, 480/2);

	bool quit = false;
	SDL_Event e;


	Timer frameTimer;
	float deltaTime = 0;

	while(!quit)
	{
		while(SDL_PollEvent(&e) != 0)
		{
			if( e.type == SDL_QUIT )
			{
				quit = true;
			}
		}
		deltaTime = frameTimer.read();
		frameTimer.reset();
		// printf("deltaTime: %f\n", deltaTime);
		// printf("Frame rate: %f\n", 1.0/deltaTime);

		gameObject.update(deltaTime);
		window.clearRenderer();

		gameObject.render(&window);
		window.presentRenderer();
	}

	close();

	return 0;
}