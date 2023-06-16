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

void __stdcall glDebugOutput(GLenum source,
                            GLenum type,
                            unsigned int id,
                            GLenum severity,
                            GLsizei length,
                            const char* message,
                            const void* userParam);


GLenum glCheckError_(const char* file, int line);

#define glCheckError() glCheckError_(__FILE__, __LINE__) 

