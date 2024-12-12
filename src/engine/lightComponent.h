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
    Vec3 direction;
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
    LightComponent();
    virtual ~LightComponent();
    virtual void start() override;
    virtual void update(float dt) override;
    virtual void render() override;
    virtual Shader& getShader() override { return *shader; };

    virtual enum ComponentType getType() override { return ComponentType::LIGHT; };
    virtual std::vector<FieldDescription>& getFields() override { return fields; };

private:
    std::vector<FieldDescription> fields = {
        { GET_NAME(shader), FieldType::ASSET_POINTER_FIELD, &shader, []() { }, ".shader" },
    };
    Shader* shader;
    Color3 color;
    LightType lightType;
    LightData lightData;
};