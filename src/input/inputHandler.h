#pragma once

#include <SDL.h>
#include <unordered_set>

// Singleton implementation from https://stackoverflow.com/questions/1008019/c-singleton-design-pattern
class InputHandler
{
public:
    static InputHandler& getInstance();
    InputHandler(InputHandler const&) = delete;
    void operator=(InputHandler const&) = delete;

    void reset();
    void addCurrentKey(SDL_Keycode keycode);
    bool queryKeyPressed(SDL_Keycode keycode);
private:
    InputHandler() {}
    std::unordered_set<SDL_Keycode> currentKeys;
};