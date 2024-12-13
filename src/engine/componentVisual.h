#pragma once

#include "component.h"
#include "shader.h"

class ComponentVisual : public Component {
public:
    ComponentVisual();
    virtual ~ComponentVisual() = default;
    virtual void start() override = 0;
    virtual void update(float dt) override = 0;
    virtual void render() = 0;

    virtual const std::vector<FieldDescription>& getFields() override { return fields; };
    virtual Shader* getShader() = 0;
    virtual bool isVisual() override { return true; }
private:
    std::vector<FieldDescription> fields = { };
};


