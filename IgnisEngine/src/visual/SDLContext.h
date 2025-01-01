#pragma once

#include <SDL.h>

class SDLContext {
public:
    SDLContext(const char* name, int width, int height);
    ~SDLContext();

    void handleEvents(bool& quit, bool& inGameView);
    void swapWindow();
    void captureMouse(bool capture);
    SDL_Window* getWindow();
    const char* glsl_version; // Should be in GLContext but whatever

private:
    SDL_Window* window;
};