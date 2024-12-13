#pragma once

#include "componentVisual.h"
#include "vec3.h"

using Color3 = Vec3; // do I want to make this its own file? Also maybe want vec4 for alpha too

enum class LightType {
    DIRECTIONAL,
    POINT,
    SPOTLIGHT
};

struct DirectionalLight {
    Vec3 direction; // Don't need direction or position, that comes from transform
};
struct PointLight {
    Vec3 position;
    float falloffConstant;
    float falloffLinear;
    float falloffQuadratic;
};
struct SpotLight {
    Vec3 position;
    Vec3 direction;
    float innerCutoff;
    float outerCutoff;
    float falloffConstant;
    float falloffLinear;
    float falloffQuadratic;
};
union LightData {
    DirectionalLight directionalLight;
    PointLight pointLight;
    SpotLight spotLight;
};

class LightComponent : public ComponentVisual {
public:
    LightComponent(LightType lightType);
    virtual ~LightComponent();
    
    virtual void start() override;
    virtual void update(float dt) override;
    virtual void render() override;
    virtual Shader* getShader() override { return shader; };

    virtual enum ComponentType getType() override { return ComponentType::LIGHT; };
    virtual const std::vector<FieldDescription>& getFields() override { return fields; };

    void setShaderUniforms(Shader& shader, int lightIndex = 0) const;

private:
    std::vector<FieldDescription> fields = {
        { GET_NAME(shader), FieldType::ASSET_POINTER_FIELD, &shader, []() { }, ".shader" },
        { GET_NAME(color), FieldType::VEC3_FIELD, &color},
        { GET_NAME(lightType), FieldType::INT_FIELD, &lightType}, // not sure how I can make this work, do i need a custom enum field where i pass in the options
    };

    Shader* shader = nullptr; // Do I need this? I think it's only necessary if the light itself is displayed, which I might want later
    Color3 color = { 1.0f, 1.0f, 1.0f };
    LightType lightType = LightType::DIRECTIONAL;
    LightData lightData = { .directionalLight = { { 1.0f, 0.0f, 0.0f } } };
};