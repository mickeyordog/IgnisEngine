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
    delete transform;
}

void GameObject::start()
{
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
    for (auto& component : this->components) {
        if (component->isActive)
            component->update(dt);
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
    component->gameObject = this;
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

