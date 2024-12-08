#pragma once

#include <stdio.h>
#include <vector>
#include <glm/gtc/matrix_transform.hpp>
#include "serialization.h"
#include "quat.h"

struct Vec3 {
public:
    Vec3() : data(0.0, 0.0, 0.0) { };
    Vec3(float x, float y, float z) : data(x, y, z) { };
    Vec3(glm::vec3 data) : data(data) { };

    float& x() { return data.x; }
    float& y() { return data.y; }
    float& z() { return data.z; }
    void normalize() { this->data = glm::normalize(data); };

    const glm::vec3& getData() const { return this->data; }
    Vec3& operator*=(float x) { data.x *= x; data.y *= x; data.z *= x; return *this; }
    // This might not be technically correct b/c the order might matter, here I'm switching them inside this function
    Vec3& operator*=(Quat q) { data = q.getData() * data; return *this; }

    static const Vec3 RIGHT;
    static const Vec3 UP;
    static const Vec3 FORWARD;

private:
    glm::vec3 data;
    // NB: "fields" not needed here because it's just accessed through Vec3 ptr
};