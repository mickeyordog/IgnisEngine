#pragma once

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