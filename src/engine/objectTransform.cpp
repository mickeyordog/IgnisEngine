#include "objectTransform.h"

#include <iostream>

ObjectTransform::ObjectTransform() /*: position(0, 0, 0), rotation(0, 0, 0), scale(0, 0, 0)*/
{
    fields.insert(fields.begin(), Component::getFields().begin(), Component::getFields().end());

    // fields.emplace_back(FieldDescription { GET_NAME(position), FieldType::Subclass, &position });
    // fields.insert(fields.end(), position.getFields().begin(), position.getFields().end());
    // fields.emplace_back(FieldDescription { GET_NAME(position), FieldType::EndSubclass, &position });

    // fields.emplace_back(FieldDescription { GET_NAME(rotation), FieldType::Subclass, &rotation });
    // fields.insert(fields.end(), rotation.getFields().begin(), rotation.getFields().end());
    // fields.emplace_back(FieldDescription { GET_NAME(rotation), FieldType::EndSubclass, &rotation });

    // fields.emplace_back(FieldDescription { GET_NAME(scale), FieldType::Subclass, &scale });
    // fields.insert(fields.end(), scale.getFields().begin(), scale.getFields().end());
    // fields.emplace_back(FieldDescription { GET_NAME(scale), FieldType::EndSubclass, &scale });

}

ObjectTransform::~ObjectTransform()
{

}

void ObjectTransform::start()
{
    std::cout << "Transform start" << std::endl;
}

void ObjectTransform::update(float dt)
{

}

void ObjectTransform::translate(Vec3 translation)
{
    this->matrix = glm::translate(this->matrix, translation.getData());
}

void ObjectTransform::scale(Vec3& scale)
{
    this->matrix = glm::scale(matrix, scale.getData());
}

void ObjectTransform::rotateAround(Vec3& axis, float angleDegrees)
{
    this->matrix = glm::rotate(this->matrix, glm::radians(angleDegrees), glm::normalize(axis.getData()));
}

// TODO: figure out how to efficiently take in new Vec3 as param, as this will copy which is maybe fine
void ObjectTransform::lookAt(Vec3 target, Vec3 up)
{
    this->matrix = glm::lookAt(glm::vec3(this->matrix[3]), target.getData(), up.getData());
}

void ObjectTransform::addChildTransform(ObjectTransform& transform)
{
    this->childTransforms.push_back(&transform);
    transform.parentTransform = this;
}