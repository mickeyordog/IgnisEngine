#pragma once

#include <vector>
#include <memory>
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
    const std::vector<std::unique_ptr<Component>>& getComponents() { return this->components; }

    bool isActive = true;
    
    ObjectTransform transform;

    const char* name;
    GameObject* parentObject;

private:
    std::vector<std::unique_ptr<Component>> components;
};