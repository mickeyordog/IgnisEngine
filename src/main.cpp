#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <stdio.h>
#include <iostream>
#include <math.h>
#include "SDLContext.h"
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

	Timer frameTimer;
	float deltaTime = 0;
	SDL_Event e;

	bool quit = false;
	while(!quit)
	{
		while (SDL_PollEvent(&e) != 0)
		{
			if (e.type == SDL_QUIT)
			{
				quit = true;
			}
		}
		InputHandler::getInstance().updateKeys();

		deltaTime = frameTimer.read();
		frameTimer.reset();
		std::cout << 1.0f/deltaTime << " fps" << std::endl;

		glContext.clear();
		geometry.render();
		sdlContext.swapWindow();
	}

	return 0;
}