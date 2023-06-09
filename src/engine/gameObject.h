#pragma once

#include <vector>
#include <memory>
#include "component.h"
#include "componentVisual.h"
#include "objectTransform.h"
#include "asset.h"
#include "serialization.h"

class GameObject {
public:
    GameObject(const char* name);
    GameObject(const char* name, FileID fileID);
    ~GameObject();

    void start();
    void update(float dt);
    void render();

    void addComponentOfType(ComponentType type);
    void removeComponent(Component& component);
    Component* getComponentOfType(ComponentType type);
    const std::vector<std::unique_ptr<Component>>& getComponents() { return this->components; }

    bool isActive = true;
    
    ObjectTransform transform;

    const char* name;
    FileID fileID;
    GameObject* parentObject;

    std::vector<FieldDescription> fields = {
        { GET_NAME(name), FieldType::STRING_FIELD, &name },
        { GET_NAME(isActive), FieldType::BOOL_FIELD, &isActive },
    };

private:
    std::vector<std::unique_ptr<Component>> components;
};