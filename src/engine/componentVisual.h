#pragma once

#include "component.h"

class ComponentVisual : public Component {
public:
    ComponentVisual();
    ~ComponentVisual();
    virtual void start() = 0;
    virtual void update(float dt) = 0;
    virtual void render() = 0;

    virtual std::vector<FieldDescription>& getFields() { return fields; };

private:
    std::vector<FieldDescription> fields = { };
};


