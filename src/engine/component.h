#pragma once

#include <vector>
#include "serialization.h"
// #include "gameObject.h"

class GameObject;

// TODO: if need List of smth that needs to be in inspector, prob make SerializedList vector class wrapper that will spit out correct fields vec
class Component {
public:
    Component();
    virtual ~Component() = default;
    virtual void start() = 0;
    virtual void update(float dt) = 0;
    virtual enum ComponentType getType() = 0;

    virtual std::vector<FieldDescription>& getFields() { return fields; };
    GameObject* parentGameObject;

    virtual bool isVisual() { return false; }
    virtual bool isPy() { return false; }
    bool isActive = true;

private:
    std::vector<FieldDescription> fields = { 
        { "isActive", FieldType::BOOL_FIELD, &isActive }
    };
};


