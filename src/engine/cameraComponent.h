#pragma once

#include <glm/gtc/matrix_transform.hpp>
#include "component.h"
#include "scene.h"
#include "renderTexture.h"

class CameraComponent : public Component {
public:
    CameraComponent(int width, int height, bool orthographic = false);
    ~CameraComponent();

    virtual void start() override;
    virtual void update(float dt) override;

    virtual std::vector<FieldDescription>& getFields() override { return fields; };
    virtual enum ComponentType getType() override { return ComponentType::CAMERA; };

    void renderScene(const Scene& scene);
    RenderTexture& getOutputTexture() { return outputTexture; }

private:
    void setProjectionMatrix();
    void updateOutputTexture();

    std::vector<FieldDescription> fields = { 
        { "width", FieldType::INT_FIELD, &width, [&](){ this->updateOutputTexture(); } },
        { "height", FieldType::INT_FIELD, &height, [&]() { this->updateOutputTexture(); } },
        { "orthographic", FieldType::BOOL_FIELD, &orthographic, [&]() { this->setProjectionMatrix(); } }
    };
    RenderTexture outputTexture;
    glm::mat4 projectionMatrix;
    int width, height; // TODO: these can't really be changed b/c being used to create render texture. Could recreate it maybe?
    bool orthographic;
};