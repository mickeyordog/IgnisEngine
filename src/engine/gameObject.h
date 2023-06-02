#pragma once

#include <vector>
#include "component.h"
#include "componentVisual.h"
#include "objectTransform.h"

class GameObject {
public:
    GameObject(const char* name);
    ~GameObject();

    void update(float dt);
    void render();

    void addComponent(Component* component); // TODO: This should prob just construct the component based on its type?
    void removeComponent(Component& component); // Should look for component of given type and remove it
    const std::vector<Component*>& getComponents() { return this->components; }

    void addVisualComponent(ComponentVisual* visualComponent);
    const std::vector<ComponentVisual*>& getVisualComponents() { return this->visualComponents; }

    
    ObjectTransform transform;

    const char* name;
    GameObject* parentObject;

private:
    std::vector<Component*> components;
    std::vector<ComponentVisual*> visualComponents;
};