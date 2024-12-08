#pragma once

#include <stdio.h>
#include <vector>
#include <glm/gtx/quaternion.hpp>
#include "serialization.h"

struct Mat4;
struct Vec3;

struct Quat {
public:
    Quat() : data() { };
    Quat(glm::quat data) : data(data) { };

    Mat4 toMat4() const;

    void lookAt(const Vec3& position, const Vec3& target, const Vec3& up);
    void rotateAboutAxis(const Vec3& axis, float angleDegrees);
    void rotateAboutAxisLocal(const Vec3& axis, float angleDegrees);

    static Vec3 toEuler(const Quat& quat);
    static Quat fromEuler(const Vec3& eulerRotationDegrees);

    const glm::quat& getData() const { return this->data; }

private:
    glm::quat data;
};