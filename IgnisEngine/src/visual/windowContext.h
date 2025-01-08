#pragma once

#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>

#define IGNIS_ERROR(str) WindowContext::showError((std::string(__FILE__) + " LINE " + std::to_string(__LINE__) + ": " + std::string(str)).c_str())

class WindowContext {
#pragma region Singleton
public:
    static WindowContext& getInstance() {
        static WindowContext instance;
        return instance;
    }
    WindowContext(WindowContext const&) = delete;
    void operator=(WindowContext const&) = delete;
private:
    WindowContext() {}
#pragma endregion
public:
    static void init(const char* name, int width, int height);
    static void deinit();

    void handleEvents(bool& quit, bool& inGameView);
    void swapWindow();
    void captureMouse(bool capture);
    SDL_Window* getWindow();
    static void showError(const char* message); // TODO: this should be called with macro so we can get line # and file name. Would be nice to also show stack trace

    const char* glsl_version = ""; // Should be in GLContext but whatever

private:
    SDL_Window* window = nullptr;
};