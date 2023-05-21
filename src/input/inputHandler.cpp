#include "inputHandler.h"

InputHandler& InputHandler::getInstance()
{
    // Guaranteed to be destroyed, instantiated on first use.
    static InputHandler instance;
    return instance;
}

void InputHandler::updateKeys()
{
    
}
