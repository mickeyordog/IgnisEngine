#pragma once

#include <SDL.h>
#include "sprite.h"
#include "renderer.h"

class Window {
public:
    Window(const char* name, int width, int height);
    ~Window();

    SDL_Window* getWindow();
private:
    SDL_Window* window;
};