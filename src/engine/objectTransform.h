#pragma once

#include "component.h"
#include "vec3.h"

#include <stdio.h>

namespace ignis_engine {
class Transform: public Component {
public:
    Transform(float x, float y, float z) : position(x,y,z), rotation(0,0,0), scale(0,0,0) {
        fields.insert(fields.begin(), Component::getFields().begin(), Component::getFields().end());

        fields.emplace_back(FieldDescription{GET_NAME(position), FieldType::Subclass, &position});
        fields.insert(fields.end(), position.getFields().begin(), position.getFields().end());
        fields.emplace_back(FieldDescription{GET_NAME(position), FieldType::EndSubclass, &position});

        fields.emplace_back(FieldDescription{GET_NAME(rotation), FieldType::Subclass, &rotation});
        fields.insert(fields.end(), rotation.getFields().begin(), rotation.getFields().end());
        fields.emplace_back(FieldDescription {GET_NAME(rotation), FieldType::EndSubclass, &rotation});

        fields.emplace_back(FieldDescription{GET_NAME(scale), FieldType::Subclass, &scale});
        fields.insert(fields.end(), scale.getFields().begin(), scale.getFields().end());
        fields.emplace_back(FieldDescription{GET_NAME(scale), FieldType::EndSubclass, &scale});
    };
    ~Transform();

    virtual void start() override;
    virtual void update(float dt) override;

    Vec3 position;
    Vec3 rotation;
    Vec3 scale;

    virtual std::vector<FieldDescription>& getFields() override { return fields; };

private:
    std::vector<FieldDescription> fields = {
        {GET_NAME(Transform), FieldType::ComponentType, this}
    };
};
}