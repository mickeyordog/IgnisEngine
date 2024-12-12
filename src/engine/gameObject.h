#pragma once

#include <vector>
#include <memory>
#include <string>
#include "component.h"
#include "componentVisual.h"
#include "transformComponent.h"
#include "asset.h"
#include "serialization.h"

class GameObject {
public:
    GameObject(std::string name, FileID fileID = -1);
    ~GameObject();

    void start();
    void update(float dt);
    void render();

    Component* addComponentOfType(ComponentType type);
    void removeComponent(Component& component);
    Component* getComponentOfType(ComponentType type);
    const std::vector<std::unique_ptr<Component>>& getComponents() { return this->components; }

    TransformComponent* transform; // TODO: can use transform->isActive instead of separate bool for gameobject

    std::string name;
    FileID fileID;

    std::vector<FieldDescription> fields = {
        { GET_NAME(name), FieldType::STRING_FIELD, &name },
    };

private:
    std::vector<std::unique_ptr<Component>> components;
};