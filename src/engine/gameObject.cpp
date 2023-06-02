#include <SDL.h>
#include "gameObject.h"
#include "inputHandler.h"
#include "objectTransform.h"

GameObject::GameObject(const char* name) : name(name), transform() {
    this->transform.parentGameObject = this;
}

GameObject::~GameObject() {
    // ~sprite(); not needed anymore
}

void GameObject::update(float dt) {
    // TODO: this causes weird behavior on hold down
    // x += InputHandler::getInstance().queryKeyPressed(SDLK_d) ? 1000 * dt : 0;
    // x -= InputHandler::getInstance().queryKeyPressed(SDLK_a) ? 1000 * dt : 0;
    // x = 100 + 100 * cos(SDL_GetTicks() / 1000.0);
    // y = 100 + 100 * sin(SDL_GetTicks() / 1000.0);
}

void GameObject::render() {
    // loop through vis components and call render
    // but how should I set uniforms?
}

void GameObject::addComponent(Component* component) {
    this->components.push_back(component);
    component->parentGameObject = this;
}

void GameObject::addVisualComponent(ComponentVisual* visualComponent)
{
    this->visualComponents.push_back(visualComponent);
    visualComponent->parentGameObject = this;
}