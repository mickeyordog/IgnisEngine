#include "lightComponent.h"
#include "gameObject.h"

LightComponent::LightComponent(LightType lightType)
{ 
    fields.insert(fields.begin(), ComponentVisual::getFields().begin(), ComponentVisual::getFields().end());

    switch (lightType) {
        case LightType::DIRECTIONAL:
            break;
        case LightType::POINT:
            lightData.pointLight.falloffConstant = 1.0f;
            lightData.pointLight.falloffLinear = 0.09f;
            lightData.pointLight.falloffQuadratic = 0.032f;
            break;
        case LightType::SPOTLIGHT:
            lightData.spotLight.falloffConstant = 1.0f;
            lightData.spotLight.falloffLinear = 0.09f;
            lightData.spotLight.falloffQuadratic = 0.032f;
            lightData.spotLight.innerCutoff = glm::cos(glm::radians(12.5f));
            lightData.spotLight.outerCutoff = glm::cos(glm::radians(15.0f));
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
    gameObject->transform->setPosition(Vec3 { sin(time), 0.0f, cos(time) });
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
            shader.setVec3(("dirLights[" + std::to_string(lightIndex) + "].direction").c_str(), gameObject->transform->getLocalForward()); 
            shader.setVec3(("dirLights[" + std::to_string(lightIndex) + "].color").c_str(), color);
            break;
        case LightType::POINT:
            shader.setVec3(("pointLights[" + std::to_string(lightIndex) + "].position").c_str(), gameObject->transform->getPosition());
            shader.setFloat(("pointLights[" + std::to_string(lightIndex) + "].constant").c_str(), lightData.pointLight.falloffConstant);
            shader.setFloat(("pointLights[" + std::to_string(lightIndex) + "].linear").c_str(), lightData.pointLight.falloffLinear);
            shader.setFloat(("pointLights[" + std::to_string(lightIndex) + "].quadratic").c_str(), lightData.pointLight.falloffQuadratic);
            shader.setVec3(("pointLights[" + std::to_string(lightIndex) + "].color").c_str(), color);
            break;
        case LightType::SPOTLIGHT:
            shader.setVec3(("spotLights[" + std::to_string(lightIndex) + "].position").c_str(), gameObject->transform->getPosition());
            shader.setVec3(("spotLights[" + std::to_string(lightIndex) + "].direction").c_str(), gameObject->transform->getLocalForward());
            shader.setFloat(("spotLights[" + std::to_string(lightIndex) + "].innerCutoff").c_str(), lightData.spotLight.innerCutoff);
            shader.setFloat(("spotLights[" + std::to_string(lightIndex) + "].outerCutoff").c_str(), lightData.spotLight.outerCutoff);
            shader.setFloat(("spotLights[" + std::to_string(lightIndex) + "].constant").c_str(), lightData.spotLight.falloffConstant);
            shader.setFloat(("spotLights[" + std::to_string(lightIndex) + "].linear").c_str(), lightData.spotLight.falloffLinear);
            shader.setFloat(("spotLights[" + std::to_string(lightIndex) + "].quadratic").c_str(), lightData.spotLight.falloffQuadratic);
            shader.setVec3(("spotLights[" + std::to_string(lightIndex) + "].color").c_str(), color);
            break;
    }
}
