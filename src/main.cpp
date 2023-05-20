#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <stdio.h>
#include <iostream>
#include <math.h>
#include "window.h"
#include "renderer.h"
#include "gameObject.h"
#include "timer.h"
#include "inputHandler.h"
#include "shader.h"
// #include <glad/glad.h>
#include <GL/glew.h>
#include <SDL_opengl.h>
#include "stb_image.h"

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
	SDL_Init(SDL_INIT_VIDEO);

	SDL_GL_SetAttribute( SDL_GL_CONTEXT_MAJOR_VERSION, 3 );
	SDL_GL_SetAttribute( SDL_GL_CONTEXT_MINOR_VERSION, 1 );
	SDL_GL_SetAttribute( SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE );

	Window gWindow("Ignis Engine", 800, 600);
	SDL_GLContext context = SDL_GL_CreateContext( gWindow.getWindow() );
	if (context == NULL)
	{
		printf("OpenGL context could not be created! SDL Error: %s\n", SDL_GetError());
	}

	glewExperimental = GL_TRUE;
	GLenum glewError = glewInit();
	if (glewError != GLEW_OK)
	{
		printf("Error initializing GLEW! %s\n", glewGetErrorString(glewError));
	}
	// glViewport(0, 0, 800, 600);

	float vertices[] = { // x, y, z, u, v
		0.5f, 	0.5f,	 0.0f, 		1.0f, 1.0f,	// top right
		0.5f, 	-0.5f,	 0.0f, 		1.0f, 0.0f,	// bottom right
		-0.5f, 	-0.5f,	 0.0f, 		0.0f, 0.0f, // bottom left
		-0.5f, 	0.5f, 	0.0f, 		0.0f, 1.0f	// top left
	};
	unsigned int indices[] = {
		// note that we start from 0!
		0, 1, 3, // first triangle
		1, 2, 3	 // second triangle
	};
	unsigned int VAO; // remember these all need to be deallocated later
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	GLuint VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	unsigned int EBO;
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	Shader shader("src/shader/vertex.vs", "src/shader/fragment.fs");
	shader.use();

	stbi_set_flip_vertically_on_load(true);
	int width, height, nrChannels;
	unsigned char *data = stbi_load("assets/texture.png", &width, &height, &nrChannels, 0);
	std::cout << width << " " << height << " " << nrChannels << std::endl;
	if (data == NULL)
	{
		std::cout << "Failed to load texture" << std::endl;
	}

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	unsigned int texture;
	glGenTextures(1, &texture);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);
	stbi_image_free(data);


	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	// glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	

	bool quit2 = false;
	while( !quit2 )
	{
		//Handle events on queue
		SDL_Event e;
		while( SDL_PollEvent( &e ) != 0 )
		{
			//User requests quit
			if( e.type == SDL_QUIT )
			{
				quit2 = true;
			}
		}
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		shader.use();
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		SDL_GL_SwapWindow(gWindow.getWindow());
	}

	return 0;

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
	SDL_Texture* texture0 = SDL_CreateTextureFromSurface(renderer.getRenderer(), surface);
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