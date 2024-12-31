#include <iostream>
#include <SDL.h>
#include "gameObject.h"
#include "inputHandler.h"
#include "serializationHelper.h"
#include "transformComponent.h"
#include "randomNumberGenerator.h"

GameObject::GameObject(std::string name, FileID fileID) : name(name)
{
    if (fileID == -1)
        fileID = RandomNumberGenerator::getRandomInteger();
    this->fileID = fileID;

    TransformComponent* transformPtr = (TransformComponent*)addComponentOfType(ComponentType::TRANSFORM);
    this->transform = transformPtr;
    this->transform->gameObject = this;
}

GameObject::~GameObject() {
    // delete transform; // TODO: this is not working, only destroying first
    std::cout << "GameObject " << this->name << " destroyed" << std::endl;
}

void GameObject::start()
{
    // TODO: if not active should I not do this?
    for (auto& component : this->components)
    {
        component->start();
    }
}

void GameObject::update(float dt) {
    // x += InputHandler::getInstance().queryKeyPressed(SDLK_d) ? 1000 * dt : 0;
    // x -= InputHandler::getInstance().queryKeyPressed(SDLK_a) ? 1000 * dt : 0;
    // x = 100 + 100 * cos(SDL_GetTicks() / 1000.0);
    // y = 100 + 100 * sin(SDL_GetTicks() / 1000.0);
    if (!transform->isActive)
        return;

    for (auto& component : this->components) {
        if (component->isActive)
            component->update(dt);
    }
}

void GameObject::fixedUpdate(float dt) { 
    if (!transform->isActive)
        return;

    for (auto& component : this->components) {
        if (component->isActive)
            component->fixedUpdate(dt);
    }
}

void GameObject::render() {
    // loop through vis components and call render
    // but how should I set uniforms?
}

Component* GameObject::addComponentOfType(ComponentType type)
{
    Component* component = SerializationHelper::getNewComponent(type);
    this->components.push_back(std::unique_ptr<Component>(component));
    component->gameObject = this; // Problem: gameObject is not set when the ctor of component is running
    return component;
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

