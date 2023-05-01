#include "gameObject.h"

GameObject::GameObject(Sprite* sprite)
{
    this->sprite = sprite;
}

GameObject::~GameObject()
{
    delete(this->sprite);
}

void GameObject::update(float dt)
{
    
}

void GameObject::render(Window* window)
{
    window->renderSprite(this->sprite, 0, 0, 100, 100);
}
