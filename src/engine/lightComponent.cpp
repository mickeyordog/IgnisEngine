#include "lightComponent.h"

LightComponent::LightComponent(LightType lightType)
{ 
    fields.insert(fields.begin(), ComponentVisual::getFields().begin(), ComponentVisual::getFields().end());

    switch (lightType) {
        case LightType::DIRECTIONAL:
            break;
        case LightType::POINT:
            break;
        case LightType::SPOTLIGHT:
            break;
    }
}

LightComponent::~LightComponent()
{ 

}

void LightComponent::start()
{ 

}

void LightComponent::update(float dt)
{ 

}

void LightComponent::render()
{

}

void LightComponent::setShaderUniforms(Shader& shader, int lightIndex) const
{ 
    switch (lightType) {
        case LightType::DIRECTIONAL:
            // could maybe cache these string values somewhere if they're expensive to compute
            shader.setVec3(("dirLights[" + std::to_string(lightIndex) + "].direction").c_str(), lightData.directionalLight.direction);
            shader.setVec3(("dirLights[" + std::to_string(lightIndex) + "].color").c_str(), color);
            break;
        case LightType::POINT:
            break;
        case LightType::SPOTLIGHT:
            break;
    }
}
