#pragma once

#include "component.h"

class Transform: public Component {
public:
    Transform(float x, float y, float z) : x(x), y(y), z(z) {
        fields.insert(fields.end(), Component::getFields().begin(), Component::getFields().end());
    };
    ~Transform();

    void start();
    void update(float dt);

    float x, y, z;

    const std::vector<FieldDescription> &getFields() { return fields; };

private:
    std::vector<FieldDescription> fields = {
        FieldDescription{GET_NAME(x), Type::Float, &x},
        FieldDescription{GET_NAME(y), Type::Float, &y},
        FieldDescription{GET_NAME(z), Type::Float, &z}
    };
};