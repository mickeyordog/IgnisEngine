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

void TransformComponent::setPosition(const Vec3& position)
{
    this->position = position;
    updateMatrix();
}

void TransformComponent::translate(const Vec3& translation)
{
    this->position += translation;
    updateMatrix();
}

void TransformComponent::translateLocal(const Vec3& translation)
{ 
    Vec3 globalTranslation = translation;
    globalTranslation.applyRotation(rotation);
    translate(globalTranslation);
}

void TransformComponent::setScale(const Vec3& scale)
{
    this->scale = scale;
    updateMatrix();
}

void TransformComponent::setEulerRotation(const Vec3& eulerRotationDegrees)
{
    rotation = Quat::fromEuler(eulerRotationDegrees);
    updateMatrix();
}

void TransformComponent::rotateAround(const Vec3& axis, float angleDegrees)
{
    rotation.rotateAboutAxis(axis, angleDegrees);
    updateMatrix();
}

void TransformComponent::rotateAroundLocal(const Vec3& axis, float angleDegrees)
{
    rotation.rotateAboutAxisLocal(axis, angleDegrees);
    updateMatrix();
}

void TransformComponent::lookAt(const Vec3& target, const Vec3& up)
{
    rotation.lookAt(position, target, up);
    updateMatrix();
}

void TransformComponent::addChildTransform(TransformComponent* transform)
{
    this->childTransforms.push_back(transform);
    transform->parentTransform = this;
}

void TransformComponent::removeChildTransform(TransformComponent* transform)
{ 
    // TODO
}

void TransformComponent::setParentMatrix(const Mat4& newParentMatrix)
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
    Mat4 translationMatrix = Mat4(1.0f);
    translationMatrix.translate(position);
    Mat4 rotationMatrix = rotation.toMat4();
    Mat4 scaleMatrix = Mat4(1.0f);
    scaleMatrix.scale(scale);
    this->globalMatrix = parentMatrix * translationMatrix * rotationMatrix * scaleMatrix;

    // TODO: This should only really happen in editor
    guiEulerAngles = Quat::toEuler(rotation);

    updateChildTransforms();
}