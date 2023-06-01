#pragma once

#include <SDL.h>

class SDLContext {
public:
    SDLContext(const char* name, int width, int height);
    ~SDLContext();

    void handleEvents(SDL_Event* e);
    void swapWindow();
    SDL_Window* getWindow();
    const char* glsl_version; // Should be in GLContext but whatever

private:
    SDL_Window* window;
};