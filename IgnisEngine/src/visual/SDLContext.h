#pragma once

#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>

class SDLContext {
//#pragma region Singleton
//public:
//    static SDLContext& getInstance() {
//        static SDLContext instance;
//        return instance;
//    }
//    SDLContext(SDLContext const&) = delete;
//    void operator=(SDLContext const&) = delete;
//private:
//    SDLContext() {}
//#pragma endregion
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