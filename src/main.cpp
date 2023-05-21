#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <stdio.h>
#include <iostream>
#include <math.h>
#include "SDLContext.h"
#include "renderer.h"
#include "gameObject.h"
#include "timer.h"
#include "inputHandler.h"
#include "shader.h"
#include <GL/glew.h>
#include <SDL_opengl.h>
#include "stb_image.h"
#include "GLContext.h"
#include "texture.h"
#include "geometry.h"

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
	TTF_Quit();
	SDL_Quit();
}

// TODO: extension that lets you add includes more easily
int main( int argc, char* args[] )
{
	SDLContext sdlContext("Ignis Engine", 800, 600);
	GLContext glContext(&sdlContext);

	// glViewport(0, 0, 200, 100);

	Texture texture("assets/texture.png");
	Shader shader("src/shader/vertex.vs", "src/shader/fragment.fs");
	Geometry geometry(&texture, &shader);

	bool quit = false;
	while( !quit )
	{
		SDL_Event e;
		while( SDL_PollEvent( &e ) != 0 )
		{
			if( e.type == SDL_QUIT )
			{
				quit = true;
			}
		}
		glContext.clear();
		geometry.render();
		sdlContext.swapWindow();
	}

	return 0;

	if(!init())
	{
		printf("Failed to initialize!\n");
		return -1;
	}

	SDLContext window("Engine", 640, 480);
	Renderer renderer(window.getWindow());
	Sprite sprite("../assets/texture.png", renderer.getRenderer());
	GameObject gameObject = GameObject(&sprite, 100, 100, 640/2, 480/2);

	bool quit2 = false;
	SDL_Event e;

	Timer frameTimer;
	float deltaTime = 0;

	// test stuff
	TTF_Font* font = TTF_OpenFont("../assets/mechanical.ttf", 28);
	SDL_Surface* surface = TTF_RenderUTF8_Solid_Wrapped(font, "Hello World!", SDL_Color{0, 0, 0, 255}, 640);
	SDL_Texture* texture0 = SDL_CreateTextureFromSurface(renderer.getRenderer(), surface);
	SDL_FreeSurface(surface);

	while(!quit)
	{
		InputHandler::getInstance().resetKeys();
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
		// printf("Frame rate: %f\n", 1.0/deltaTime);

		gameObject.update(deltaTime);
		renderer.clear();
		SDL_RenderCopy(renderer.getRenderer(), texture0, NULL, NULL);

		gameObject.render(&renderer);
		renderer.present();
	}

	close();

	return 0;
}