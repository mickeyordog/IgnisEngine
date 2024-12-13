#pragma once

#include "component.h"
#include "serialization.h"

class FirstPersonController : public Component {
public:
    FirstPersonController(float movementSpeed, float rotationSpeed);
    ~FirstPersonController();

    virtual void start() override;
    virtual void update(float dt) override;
    virtual enum ComponentType getType() override { return ComponentType::FIRST_PERSON_CONTROLLER; };

    virtual const std::vector<FieldDescription>& getFields() override { return fields; };

private:
    std::vector<FieldDescription> fields = {
        { GET_NAME(movementSpeed), FieldType::FLOAT_FIELD, &movementSpeed},
        { GET_NAME(rotationSpeed), FieldType::FLOAT_FIELD, &rotationSpeed},
    };

    float movementSpeed, rotationSpeed;
};