#include "inputHandler.h"

#include <SDL.h>

// How do I want to let the user interface with this?
// REQUIREMENTS:
// - needs to support user remappable keys with game-specific defaults
// - preferable stored as an enum
void InputHandler::updateState(const Uint8* currentKeyStates, Vec2 mouseDelta)
{
    this->currentKeyStates = currentKeyStates;
    this->mouseDelta = mouseDelta;
    this->mousePos += mouseDelta;

    if (firstUpdate) {
        this->mouseDelta = Vec2(0.0f);
        this->mousePos = Vec2(0.0f);
        firstUpdate = false;
    }
}

bool InputHandler::keyPressed(SDL_Scancode key)
{
    if (this->currentKeyStates == nullptr)
        return false;
    return this->currentKeyStates[key];
}
