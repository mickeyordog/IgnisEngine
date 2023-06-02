#pragma once

#include "component.h"

class CameraComponent : public Component {
public:
    CameraComponent();
    ~CameraComponent();

    virtual void start() override;
    virtual void update(float dt) override;

    virtual std::vector<FieldDescription>& getFields() override { return fields; };

private:
    std::vector<FieldDescription> fields = { };
};