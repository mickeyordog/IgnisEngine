#pragma once

#include <GL/glew.h>
#include <stdio.h>
#include <string>
#include <iostream>
#include "SDLContext.h"

class GLContext {
public:
    GLContext(SDLContext* window);
    ~GLContext();

    void clear(float r, float g, float b, float a);
    SDL_GLContext* getContext();
    
private:
    SDL_GLContext context;
};

GLenum glCheckError_(const char* file, int line);

#define glCheckError() glCheckError_(__FILE__, __LINE__) 