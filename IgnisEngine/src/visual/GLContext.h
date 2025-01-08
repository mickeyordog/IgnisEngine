#pragma once

#include <GL/glew.h>
#include <stdio.h>
#include <string>
#include <iostream>
#include "windowContext.h"

class GLContext {
public:
    GLContext(WindowContext* windowContext);
    ~GLContext();

    void clear(float r, float g, float b, float a);
    SDL_GLContext* getContext();
    
private:
    SDL_GLContext context;
};

void GLAPIENTRY glDebugOutput(GLenum source,
                            GLenum type,
                            unsigned int id,
                            GLenum severity,
                            GLsizei length,
                            const char* message,
                            const void* userParam);


GLenum glCheckError_(const char* file, int line);

#define glCheckError() glCheckError_(__FILE__, __LINE__) 

