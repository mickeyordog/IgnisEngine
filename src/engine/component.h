#pragma once

#include <vector>
#include "serialization.h"
#include "asset.h"

class GameObject;
// TODO: if need List of smth that needs to be in inspector, prob make SerializedList vector class wrapper that will spit out correct fields vec
class Component {
public:
    Component();
    virtual ~Component() = default;
    virtual void start() { }; // TODO: this is not currently being called if added after start of game
    virtual void update(float dt) { }; // Potential problem: not calling update on base class
    virtual void fixedUpdate(float dt) { };
    virtual enum ComponentType getType() = 0;

    void setField(const FieldDescription& field, void* value);
    virtual const std::vector<FieldDescription>& getFields() { return fields; };
    GameObject* gameObject;

    virtual bool isVisual() { return false; }
    bool isActive = true;
    FileID fileID;

private:
    std::vector<FieldDescription> fields = { 
        { GET_NAME(fileID), FieldType::LLONG_FIELD, &fileID },
        { GET_NAME(isActive), FieldType::BOOL_FIELD, &isActive }
    };
};