#include "inputHandler.h"

#include <SDL.h>

InputHandler& InputHandler::getInstance()
{
    // Guaranteed to be destroyed, instantiated on first use.
    static InputHandler instance;
    return instance;
}

// How do I want to let the user interface with this?
// REQUIREMENTS:
// - needs to support user remappable keys with game-specific defaults
// - preferable stored as an enum
void InputHandler::updateKeys()
{
    this->currentKeyStates = SDL_GetKeyboardState(NULL);
}
