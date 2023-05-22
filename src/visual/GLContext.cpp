#include <GL/glew.h>
#include "GLContext.h"
#include <stdio.h>

GLContext::GLContext(SDLContext *sdlContext)
{
    this->context = SDL_GL_CreateContext(sdlContext->getWindow());
    if (this->context == NULL)
    {
        printf("OpenGL context could not be created! SDL Error: %s\n", SDL_GetError());
    }
    if (SDL_GL_MakeCurrent(sdlContext->getWindow(), this->context) < 0)
    {
        printf("Unable to set current context! SDL Error: %s\n", SDL_GetError());
    }
    if (SDL_GL_SetSwapInterval(1) < 0)
    {
        printf("Unable to set VSync! SDL Error: %s\n", SDL_GetError());
    }
    

    glewExperimental = GL_TRUE;
    GLenum glewError = glewInit();
    if (glewError != GLEW_OK)
    {
        printf("Error initializing GLEW! %s\n", glewGetErrorString(glewError));
    }
}

GLContext::~GLContext() {
    SDL_GL_DeleteContext(this->context);
}

void GLContext::clear(float r, float g, float b, float a)
{
    glClearColor(r, g, b, a);
    glClear(GL_COLOR_BUFFER_BIT);
}

SDL_GLContext* GLContext::getContext()
{
    return &this->context;
}
