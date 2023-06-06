#include "objectTransform.h"

#include <iostream>

ObjectTransform::ObjectTransform()
{
    fields.insert(fields.begin(), Component::getFields().begin(), Component::getFields().end());
}

ObjectTransform::~ObjectTransform()
{

}

void ObjectTransform::start()
{

}

void ObjectTransform::update(float dt)
{

}

void ObjectTransform::setPosition(Vec3 position)
{
    this->position = position.getData();
    updateMatrix();
}

void ObjectTransform::translate(Vec3 translation)
{
    this->position += translation.getData();
    updateMatrix();
}

void ObjectTransform::setScale(Vec3 scale)
{
    this->scale = scale.getData();
    updateMatrix();
}

void ObjectTransform::setEulerRotation(Vec3 eulerRotationDegrees)
{
    rotation = glm::quat(glm::radians(eulerRotationDegrees.getData()));
    updateMatrix();
}

void ObjectTransform::rotateAround(Vec3& axis, float angleDegrees)
{
    // this->globalMatrix = glm::rotate(this->globalMatrix, glm::radians(angleDegrees), glm::normalize(axis.getData()));
}

void ObjectTransform::lookAt(Vec3 target, Vec3 up)
{
    glm::mat4 viewMatrix = glm::lookAt(this->position, target.getData(), up.getData());
    this->rotation = glm::quat_cast(viewMatrix);
    updateMatrix();
}

void ObjectTransform::addChildTransform(ObjectTransform& transform)
{
    this->childTransforms.push_back(&transform);
    transform.parentTransform = this;
}

void ObjectTransform::setParentMatrix(glm::mat4& newParentMatrix)
{
    this->parentMatrix = newParentMatrix;
    updateMatrix();
}

void ObjectTransform::updateChildTransforms()
{
    for (auto& child : childTransforms) {
        child->setParentMatrix(this->globalMatrix);
        child->updateChildTransforms();
    }
}

void ObjectTransform::updateMatrix()
{
    glm::mat4 translationMatrix = glm::translate(glm::mat4(1.0f), position);
    glm::mat4 rotationMatrix = glm::toMat4(rotation);
    glm::mat4 scaleMatrix = glm::scale(glm::mat4(1.0f), scale);
    this->globalMatrix = /*parentMatrix * */translationMatrix * rotationMatrix * scaleMatrix;

    updateChildTransforms();
}