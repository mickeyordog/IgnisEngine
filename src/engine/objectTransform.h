#pragma once

#include "component.h"
#include "vec3.h"

#include <stdio.h>

namespace ignis_engine {
class Transform: public Component {
public:
    Transform(float x, float y, float z) : position(x,y,z), rotation(0,0,0), scale(0,0,0) {
        fields.insert(fields.end(), Component::getFields().begin(), Component::getFields().end());
        fields.insert(fields.end(), position.getFields().begin(), position.getFields().end());
        fields.insert(fields.end(), rotation.getFields().begin(), rotation.getFields().end());
        fields.insert(fields.end(), scale.getFields().begin(), scale.getFields().end());
    };
    ~Transform();

    virtual void start() override;
    virtual void update(float dt) override;

    Vec3 position;
    Vec3 rotation;
    Vec3 scale;

    const std::vector<FieldDescription>& getFields() { return fields; };

private:
    std::vector<FieldDescription> fields = { };
};
}