#pragma once

#include <SDL.h>

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
    void updateKeys();
    bool keyPressed(SDL_Scancode key);
private:
    const Uint8* currentKeyStates;
};