#pragma once

#include <SDL.h>

class SDLContext {
public:
    SDLContext(const char* name, int width, int height);
    ~SDLContext();

    void handleEvents(SDL_Event* e);
    void swapWindow();
    SDL_Window* getWindow();
private:
    SDL_Window* window;
};