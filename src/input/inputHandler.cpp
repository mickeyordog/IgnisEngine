#include "inputHandler.h"

#include <SDL.h>

// How do I want to let the user interface with this?
// REQUIREMENTS:
// - needs to support user remappable keys with game-specific defaults
// - preferable stored as an enum
void InputHandler::updateKeys()
{
    int numKeys;
    this->currentKeyStates = SDL_GetKeyboardState(&numKeys);
}
