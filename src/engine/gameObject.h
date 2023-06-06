#pragma once

#include <vector>
#include "component.h"
#include "componentVisual.h"
#include "objectTransform.h"

class GameObject {
public:
    GameObject(const char* name);
    ~GameObject();

    void start();
    void update(float dt);
    void render();

    void addComponentOfType(ComponentType type);
    void removeComponent(Component& component); // Should look for component of given type and remove it
    Component* getComponentOfType(ComponentType type);
    const std::vector<Component*>& getComponents() { return this->components; }

    bool isActive = true;
    
    ObjectTransform transform;

    const char* name;
    GameObject* parentObject;

private:
    // TODO: have these be same list, just have virtual method that defines whether it is visual or not
    void addComponent(Component* component);
    std::vector<Component*> components;
};