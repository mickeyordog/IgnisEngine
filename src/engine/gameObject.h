#pragma once

#include <vector>
#include "component.h"

class GameObject {
public:
    GameObject(const char* name);
    ~GameObject();

    void update(float dt);
    void render();

    void addComponent(Component& component); // TODO: This should prob just construct the component based on its type?
    void removeComponent(Component& component); // Should look for component of given type and remove it
    std::vector<Component*>& getComponents();

    void addChildObject(GameObject& gameObject);
    void removeChildObject(GameObject& gameObject);
    std::vector<GameObject*>& getChildObjects();

    const char* name;
    GameObject* parentObject;

private:
    int width, height; // These could maybe be moved to sprite class
    std::vector<Component*> components;
    std::vector<GameObject*> childObjects;
};