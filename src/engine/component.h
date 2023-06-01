#pragma once

#include <vector>
#include "serialization.h"
#include "gameObject.h"

// TODO: if need List of smth that needs to be in inspector, prob make SerializedList vector class wrapper that will spit out correct fields vec
class Component {
public:
    Component();
    ~Component();
    virtual void start() = 0;
    virtual void update(float dt) = 0;

    virtual std::vector<FieldDescription>& getFields() { return fields; };
    GameObject* parentGameObject;

private:
    std::vector<FieldDescription> fields = { };
};


