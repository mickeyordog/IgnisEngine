#pragma once

#include "component.h"

class Camera : public Component {
public:
    Camera();
    ~Camera();

    virtual void start() override;
    virtual void update(float dt) override;

    virtual std::vector<FieldDescription>& getFields() override { return fields; };

private:
    std::vector<FieldDescription> fields = { };
};