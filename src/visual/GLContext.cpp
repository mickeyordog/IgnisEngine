#include <GL/glew.h>
#include "GLContext.h"
#include <stdio.h>

GLContext::GLContext(SDLContext *window)
{
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    if (SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1) < 0)
    {
        printf("Unable to set double buffer! SDL Error: %s\n", SDL_GetError());
    }

    SDL_GLContext context = SDL_GL_CreateContext(window->getWindow());
    if (context == NULL)
    {
        printf("OpenGL context could not be created! SDL Error: %s\n", SDL_GetError());
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

}

void GLContext::clear()
{
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
}
