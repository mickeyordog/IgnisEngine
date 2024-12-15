#include "lightComponent.h"
#include "gameObject.h"

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
    static float time = 0.0f;
    time += dt;
    gameObject->transform->setPosition(Vec3 { 5.0f * sin(time), 5.0f, 5.0f * cos(time) });
    gameObject->transform->lookAt(Vec3 { 0.0f, 0.0f, 0.0f });
}

void LightComponent::render()
{

}

void LightComponent::setShaderUniforms(Shader& shader, int lightIndex) const
{ 
    switch (lightType) {
        case LightType::DIRECTIONAL:
            // could maybe cache these string values somewhere if they're expensive to compute
            // This should be passing in 
            shader.setVec3(("dirLights[" + std::to_string(lightIndex) + "].direction").c_str(), gameObject->transform->getLocalForward()); 
            shader.setVec3(("dirLights[" + std::to_string(lightIndex) + "].color").c_str(), color);
            break;
        case LightType::POINT:
            break;
        case LightType::SPOTLIGHT:
            break;
    }
}
