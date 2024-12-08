#pragma once

#include <stdio.h>
#include <vector>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>
#include "serialization.h"
#include "vec3.h"

struct Mat4 {
public:
    Mat4(float x) : data(x) { };
    Mat4(const glm::mat4& data) : data(data) { };

    void translate(const Vec3& translation) { data = glm::translate(data, translation.getData()); };
    void scale(const Vec3& scale) { data = glm::scale(data, scale.getData()); };

    void invert() { data = glm::inverse(data); };

    const glm::mat4& getData() const { return this->data; }

private:
    glm::mat4 data;
};

inline Mat4 operator*(const Mat4& a, const Mat4& b)
{
    return Mat4(a.getData() * b.getData());
}

inline std::ostream& operator<<(std::ostream& str, Mat4 const& v)
{
    str << glm::to_string(v.getData());
    return str;
}