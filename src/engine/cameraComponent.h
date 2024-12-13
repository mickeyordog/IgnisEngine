#pragma once

#include <glm/gtc/matrix_transform.hpp>
#include <memory>
#include "component.h"
#include "renderTexture.h"
#include "mat4.h"

class Scene;
class CameraComponent : public Component {
public:
    CameraComponent(int width, int height, bool orthographic = false);
    ~CameraComponent();

    virtual void start() override;
    virtual void update(float dt) override;

    virtual const std::vector<FieldDescription>& getFields() override { return fields; };
    virtual enum ComponentType getType() override { return ComponentType::CAMERA; };

    void renderScene(Scene& scene);
    RenderTexture& getOutputTexture() { return *outputTexture; }

private:
    void setProjectionMatrix();
    void updateOutputTexture();

    std::vector<FieldDescription> fields = { 
        { "width", FieldType::INT_FIELD, &width, [&](){ this->updateOutputTexture(); } },
        { "height", FieldType::INT_FIELD, &height, [&]() { this->updateOutputTexture(); } },
        { "orthographic", FieldType::BOOL_FIELD, &orthographic, [&]() { this->setProjectionMatrix(); } }
    };
    RenderTexture* outputTexture = nullptr;
    Mat4 projectionMatrix = Mat4(1.0f);
    int width, height;
    bool orthographic;
};