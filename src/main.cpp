#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <stdio.h>
#include <math.h>
#include "window.h"
#include "renderer.h"
#include "gameObject.h"
#include "timer.h"
#include "inputHandler.h"


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
	if( TTF_Init() == -1 )
	{
		printf( "SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError() );
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
	Renderer renderer(window.getWindow());
	Sprite sprite("../assets/texture.png", renderer.getRenderer());
	GameObject gameObject = GameObject(&sprite, 100, 100, 640/2, 480/2);

	bool quit = false;
	SDL_Event e;

	Timer frameTimer;
	float deltaTime = 0;

	// test stuff
	TTF_Font* font = TTF_OpenFont("../assets/mechanical.ttf", 28);
	SDL_Surface* surface = TTF_RenderUTF8_Solid_Wrapped(font, "Hello World!", SDL_Color{0, 0, 0, 255}, 640);
	SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer.getRenderer(), surface);
	SDL_FreeSurface(surface);

	while(!quit)
	{
		InputHandler::getInstance().reset();
		while(SDL_PollEvent(&e) != 0)
		{
			if(e.type == SDL_QUIT)
			{
				quit = true;
			}
			else if(e.type == SDL_KEYDOWN)
			{
				InputHandler::getInstance().addCurrentKey(e.key.keysym.sym);
			}
		}
		deltaTime = frameTimer.read();
		frameTimer.reset();
		// printf("deltaTime: %f\n", deltaTime);
		printf("Frame rate: %f\n", 1.0/deltaTime);

		gameObject.update(deltaTime);
		renderer.clear();
		SDL_RenderCopy(renderer.getRenderer(), texture, NULL, NULL);

		gameObject.render(&renderer);
		renderer.present();
	}

	close();

	return 0;
}