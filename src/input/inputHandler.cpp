#include "inputHandler.h"

InputHandler& InputHandler::getInstance()
{
    // Guaranteed to be destroyed, instantiated on first use.
    static InputHandler instance;
    return instance;
}

// TODO: rename to resetKeys
void InputHandler::reset()
{
    this->currentKeys.clear();
}

void InputHandler::addCurrentKey(SDL_Keycode keycode)
{
    this->currentKeys.insert(keycode);
}

bool InputHandler::queryKeyPressed(SDL_Keycode keycode)
{
    return this->currentKeys.find(keycode) != this->currentKeys.end();
}
