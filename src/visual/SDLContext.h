#pragma once

#include <SDL.h>

class SDLContext {
public:
    SDLContext(const char* name, int width, int height);
    ~SDLContext();

    void swapWindow();
    SDL_Window* getWindow();
private:
    SDL_Window* window;
};