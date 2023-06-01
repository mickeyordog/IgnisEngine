#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "vec3.h"

class TransformationMatrix {
public:
    TransformationMatrix() { matrix = glm::mat4(1.0f); };
    ~TransformationMatrix();

    void translate(Vec3& translation);
    void scale(Vec3& scale);
    void rotateAround(Vec3& axis, float angleDegrees);
    void lookAt(Vec3& target, Vec3& up);

private:
    glm::mat4 matrix;
};