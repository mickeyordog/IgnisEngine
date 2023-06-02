#include <SDL.h>
#include "gameObject.h"
#include "inputHandler.h"

GameObject::GameObject(const char* name) : name(name) {

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

}

void GameObject::addComponent(Component& component) {
    this->components.push_back(&component);
    component.parentGameObject = this; // TODO: add this back when working
}

std::vector<Component*>& GameObject::getComponents() {
    return this->components;
}

void GameObject::addChildObject(GameObject& gameObject) {
    this->childObjects.push_back(&gameObject);
    gameObject.parentObject = this;
}

std::vector<GameObject*>& GameObject::getChildObjects() {
    return this->childObjects;
}