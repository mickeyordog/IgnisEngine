#include "transformComponent.h"

#include <iostream>

TransformComponent::TransformComponent()
{
    fields.insert(fields.begin(), Component::getFields().begin(), Component::getFields().end());
}

TransformComponent::~TransformComponent()
{

}

void TransformComponent::start()
{

}

void TransformComponent::update(float dt)
{

}

void TransformComponent::setPosition(Vec3 position)
{
    this->position = position.getData();
    updateMatrix();
}

void TransformComponent::translate(Vec3 translation)
{
    this->position += translation.getData();
    updateMatrix();
}

void TransformComponent::setScale(Vec3 scale)
{
    this->scale = scale.getData();
    updateMatrix();
}

void TransformComponent::setEulerRotation(Vec3 eulerRotationDegrees)
{
    rotation = glm::quat(glm::radians(eulerRotationDegrees.getData()));
    updateMatrix();
}

void TransformComponent::rotateAround(Vec3& axis, float angleDegrees)
{
    // this->globalMatrix = glm::rotate(this->globalMatrix, glm::radians(angleDegrees), glm::normalize(axis.getData()));
}

void TransformComponent::lookAt(Vec3 target, Vec3 up)
{
    glm::mat4 viewMatrix = glm::lookAt(this->position, target.getData(), up.getData());
    this->rotation = glm::quat_cast(viewMatrix);
    updateMatrix();
}

void TransformComponent::addChildTransform(TransformComponent* transform)
{
    this->childTransforms.push_back(transform);
    transform->parentTransform = this;
}

void TransformComponent::setParentMatrix(glm::mat4& newParentMatrix)
{
    this->parentMatrix = newParentMatrix;
    updateMatrix();
}

void TransformComponent::updateChildTransforms()
{
    for (auto& child : childTransforms) {
        child->setParentMatrix(this->globalMatrix);
        child->updateChildTransforms();
    }
}

void TransformComponent::updateMatrix()
{
    glm::mat4 translationMatrix = glm::translate(glm::mat4(1.0f), position);
    glm::mat4 rotationMatrix = glm::toMat4(rotation);
    glm::mat4 scaleMatrix = glm::scale(glm::mat4(1.0f), scale);
    this->globalMatrix = parentMatrix * translationMatrix * rotationMatrix * scaleMatrix;

    updateChildTransforms();
}