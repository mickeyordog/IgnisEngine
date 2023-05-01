#include <SDL.h>
#include "gameObject.h"

GameObject::GameObject(Sprite* sprite, int x, int y, int width, int height) : x(x), y(y), width(width), height(height)
{
    this->sprite = sprite;
}

GameObject::~GameObject()
{
    delete(this->sprite);
}

void GameObject::update(float dt)
{
    x = 100 + 100 * cos(SDL_GetTicks() / 1000.0);
    y = 100 + 100 * sin(SDL_GetTicks() / 1000.0);
}

void GameObject::render(Window* window)
{
    window->renderSprite(this->sprite, x, y, this->width, this->height);
}
