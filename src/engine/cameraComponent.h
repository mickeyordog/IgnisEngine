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

    void renderScene(const Scene& scene);
    RenderTexture& getOutputTexture() { return outputTexture; }

private:
    std::vector<FieldDescription> fields = { };
    RenderTexture outputTexture;
    glm::mat4 projectionMatrix;
};