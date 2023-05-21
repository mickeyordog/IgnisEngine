#pragma once

#include <SDL.h>
#include "sprite.h"
#include "renderer.h"

// TODO: rename to SDLContext
class Window {
public:
    Window(const char* name, int width, int height);
    ~Window();

    void swapWindow();
    SDL_Window* getWindow();
private:
    SDL_Window* window;
};