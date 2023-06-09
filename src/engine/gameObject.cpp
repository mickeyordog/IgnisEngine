#include <SDL.h>
#include "gameObject.h"
#include "inputHandler.h"
#include "serializationHelper.h"
#include "objectTransform.h"
#include "randomNumberGenerator.h"

GameObject::GameObject(const char* name) : name(name) {
    // Idk if necessary but could guarantee these don't collide during player gameplay
    // using a static counter, or maybe could keep track of generated ids and check for collision
    fileID = RandomNumberGenerator::getRandomInteger();
    this->transform.parentGameObject = this;
}

GameObject::GameObject(const char* name, FileID fileID) : name(name), fileID(fileID)
{
    this->transform.parentGameObject = this;
}

GameObject::~GameObject() {
    // TODO: transform? Rn being stack allocated
}

void GameObject::start()
{
    transform.start();
    for (auto& component : this->components)
    {
        component->start();
    }
}

void GameObject::update(float dt) {
    // TODO: this causes weird behavior on hold down
    // x += InputHandler::getInstance().queryKeyPressed(SDLK_d) ? 1000 * dt : 0;
    // x -= InputHandler::getInstance().queryKeyPressed(SDLK_a) ? 1000 * dt : 0;
    // x = 100 + 100 * cos(SDL_GetTicks() / 1000.0);
    // y = 100 + 100 * sin(SDL_GetTicks() / 1000.0);
    transform.update(dt);
    for (auto& component : this->components) {
        if (component->isActive)
            component->update(dt);
    }
}

void GameObject::render() {
    // loop through vis components and call render
    // but how should I set uniforms?
}

void GameObject::addComponentOfType(ComponentType type)
{
    Component* component = SerializationHelper::getNewComponent(type);
    this->components.push_back(std::unique_ptr<Component>(component));
    component->parentGameObject = this;
}

Component* GameObject::getComponentOfType(ComponentType type)
{
    for (auto& component : this->components)
    {
        if (component->getType() == type) {
            return component.get();
        }
    }
    return nullptr;
}