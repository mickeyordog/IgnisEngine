#pragma once

#include <SDL2/SDL.h>
#include "vec2.h"

// Singleton implementation from https://stackoverflow.com/questions/1008019/c-singleton-design-pattern
class InputHandler
{
#pragma region Singleton
public:
    static InputHandler& getInstance() {
        // Guaranteed to be destroyed, instantiated on first use.
        static InputHandler instance;
        return instance;
    }
    InputHandler(InputHandler const&) = delete;
    void operator=(InputHandler const&) = delete;
private:
    InputHandler() { }
#pragma endregion

public:
    void updateState(const Uint8* currentKeyStates, Vec2 mouseDelta);
    bool keyPressed(SDL_Scancode key);
    Vec2 getMousePos() { return mousePos; }
    Vec2 getMouseDelta() { return mouseDelta; }
private:
    const Uint8* currentKeyStates = nullptr;
    Vec2 mousePos = Vec2(0.0f);
    Vec2 mouseDelta = Vec2(0.0f);
    bool firstUpdate = true;
};