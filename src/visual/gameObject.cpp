#include <SDL.h>
#include "gameObject.h"
#include "inputHandler.h"

GameObject::GameObject(float x, float y, int width, int height) : x(x), y(y), width(width), height(height)
{

}

GameObject::~GameObject()
{
    // ~sprite(); not needed anymore
}

void GameObject::update(float dt)
{
    // TODO: this causes weird behavior on hold down
    // x += InputHandler::getInstance().queryKeyPressed(SDLK_d) ? 1000 * dt : 0;
    // x -= InputHandler::getInstance().queryKeyPressed(SDLK_a) ? 1000 * dt : 0;
    // x = 100 + 100 * cos(SDL_GetTicks() / 1000.0);
    // y = 100 + 100 * sin(SDL_GetTicks() / 1000.0);
}

void GameObject::render()
{
    
}
