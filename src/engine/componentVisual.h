#pragma once

#include "component.h"
#include "shader.h"

class ComponentVisual : public Component {
public:
    ComponentVisual();
    virtual ~ComponentVisual() = default;
    virtual void start() = 0;
    virtual void update(float dt) = 0;
    virtual void render() = 0;

    virtual std::vector<FieldDescription>& getFields() { return fields; };
    virtual Shader& getShader() = 0;
private:
    std::vector<FieldDescription> fields = { };
};


