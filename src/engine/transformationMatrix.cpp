#include "transformationMatrix.h"

TransformationMatrix::~TransformationMatrix()
{
    
}

void TransformationMatrix::translate(Vec3& translation)
{
    this->matrix = glm::translate(this->matrix, translation.getData());
}

void TransformationMatrix::scale(Vec3& scale)
{
    this->matrix = glm::scale(matrix, scale.getData());
}

void TransformationMatrix::rotateAround(Vec3& axis, float angleDegrees)
{
    this->matrix = glm::rotate(this->matrix, glm::radians(angleDegrees), glm::normalize(axis.getData()));
}

void TransformationMatrix::lookAt(Vec3& target, Vec3& up)
{
    this->matrix = glm::lookAt(glm::vec3(this->matrix[3]), target.getData(), up.getData());
}
