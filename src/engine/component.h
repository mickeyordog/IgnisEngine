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
    virtual void start() = 0;
    virtual void update(float dt) = 0;
    virtual enum ComponentType getType() = 0;

    void setField(const FieldDescription& field, void* value);
    virtual std::vector<FieldDescription>& getFields() { return fields; };
    GameObject* gameObject;

    virtual bool isVisual() { return false; }
    virtual bool isPy() { return false; }
    bool isActive = true;
    FileID fileID;

private:
    std::vector<FieldDescription> fields = { 
        { GET_NAME(fileID), FieldType::LLONG_FIELD, &fileID },
        { GET_NAME(isActive), FieldType::BOOL_FIELD, &isActive }
    };
};


