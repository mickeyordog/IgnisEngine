#include "GLContext.h"


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
        printf("Unable to set Swap Interval! SDL Error: %s\n", SDL_GetError());
    }
    

    glewExperimental = GL_TRUE;
    GLenum glewError = glewInit();
    if (glewError != GLEW_OK)
    {
        printf("Error initializing GLEW! %s\n", glewGetErrorString(glewError));
    }

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_DEPTH_TEST);
}

GLContext::~GLContext() {
    SDL_GL_DeleteContext(this->context);
}

void GLContext::clear(float r, float g, float b, float a)
{
    glClearColor(r, g, b, a);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

SDL_GLContext* GLContext::getContext()
{
    return &this->context;
}

GLenum glCheckError_(const char* file, int line)
{
    GLenum errorCode;
    while ((errorCode = glGetError()) != GL_NO_ERROR)
    {
        std::string error;
        switch (errorCode)
        {
        case GL_INVALID_ENUM:                  error = "INVALID_ENUM"; break;
        case GL_INVALID_VALUE:                 error = "INVALID_VALUE"; break;
        case GL_INVALID_OPERATION:             error = "INVALID_OPERATION"; break;
        case GL_STACK_OVERFLOW:                error = "STACK_OVERFLOW"; break;
        case GL_STACK_UNDERFLOW:               error = "STACK_UNDERFLOW"; break;
        case GL_OUT_OF_MEMORY:                 error = "OUT_OF_MEMORY"; break;
        case GL_INVALID_FRAMEBUFFER_OPERATION: error = "INVALID_FRAMEBUFFER_OPERATION"; break;
        }
        std::cout << error << " | " << file << " (" << line << ")" << std::endl;
    }
    return errorCode;
}

