#include "transformComponent.h"

#include <iostream>
#include "scene.h"

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

void TransformComponent::set(const Vec3& position, const Quat& rotation) { 
    this->position = position;
    this->rotation = rotation;
    guiEulerAngles = Quat::toEuler(rotation); // TODO: This should only really happen in editor
    updateMatrix();
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

void TransformComponent::setRotation(const Quat& rotation) { 
    this->rotation = rotation;
    guiEulerAngles = Quat::toEuler(rotation); // TODO: This should only really happen in editor
    updateMatrix();
}

void TransformComponent::setEulerRotation(const Vec3& eulerRotationDegrees)
{
    rotation = Quat::fromEuler(eulerRotationDegrees);
    guiEulerAngles = eulerRotationDegrees; // TODO: This should only really happen in editor
    updateMatrix();
}

void TransformComponent::rotateAround(const Vec3& axis, float angleDegrees)
{
    rotation.rotateAboutAxis(axis, angleDegrees);
    guiEulerAngles = Quat::toEuler(rotation); // TODO: This should only really happen in editor
    updateMatrix();
}

void TransformComponent::rotateAroundLocal(const Vec3& axis, float angleDegrees)
{
    rotation.rotateAboutAxisLocal(axis, angleDegrees);
    guiEulerAngles = Quat::toEuler(rotation); // TODO: This should only really happen in editor
    updateMatrix();
}

void TransformComponent::lookAt(const Vec3& target, const Vec3& up)
{
    rotation.lookAt(position, target, up);
    guiEulerAngles = Quat::toEuler(rotation); // TODO: This should only really happen in editor
    updateMatrix();
}

void TransformComponent::addChildTransform(TransformComponent* transform)
{
    this->childTransforms.push_back(transform);
    transform->parentTransform = this;
    transform->gameObject->scene = gameObject->scene;
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

    updateChildTransforms();
}