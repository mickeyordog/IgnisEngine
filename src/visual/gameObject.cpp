#include <SDL.h>
#include "gameObject.h"
#include "renderer.h"
// #include "inputHandler.h"

// Makes me kinda nervous that it's keeping a pointer to the sprite
GameObject::GameObject(Sprite* sprite, int x, int y, int width, int height) : x(x), y(y), width(width), height(height)
{
    this->sprite = sprite;
}

GameObject::~GameObject()
{
    // ~sprite(); not needed anymore
}

void GameObject::update(float dt)
{
    // x += InputHandler::getInstance().queryKeyPressed(SDLK_d) ? 10 * dt : 0;
    x = 100 + 100 * cos(SDL_GetTicks() / 1000.0);
    y = 100 + 100 * sin(SDL_GetTicks() / 1000.0);
}

void GameObject::render(Renderer* renderer)
{
    renderer->renderSprite(this->sprite, x, y, this->width, this->height);
}
