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

    void addComponent(Component* component); // TODO: This should prob just construct the component based on its type?
    void removeComponent(Component& component); // Should look for component of given type and remove it
    Component* getComponentOfType(enum ComponentType type);
    const std::vector<Component*>& getComponents() { return this->components; }

    void addVisualComponent(ComponentVisual* visualComponent);
    const std::vector<ComponentVisual*>& getVisualComponents() { return this->visualComponents; }

    bool isActive = true;
    
    ObjectTransform transform;

    const char* name;
    GameObject* parentObject;

private:
    // TODO: have these be same list, just have virtual method that defines whether it is visual or not
    std::vector<Component*> components;
    std::vector<ComponentVisual*> visualComponents;
};