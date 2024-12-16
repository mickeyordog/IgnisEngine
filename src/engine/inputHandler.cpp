#include "inputHandler.h"

#include <SDL.h>

// How do I want to let the user interface with this?
// REQUIREMENTS:
// - needs to support user remappable keys with game-specific defaults
// - preferable stored as an enum
void InputHandler::updateState(const Uint8* currentKeyStates, Vec2 mousePos)
{
    this->currentKeyStates = currentKeyStates;
    this->lastMousePos = firstUpdate ? mousePos : this->mousePos;
    this->mousePos = mousePos;
    if (this->mousePos != this->lastMousePos)
        firstUpdate = false;
}

bool InputHandler::keyPressed(SDL_Scancode key)
{
    return this->currentKeyStates[key];
}
