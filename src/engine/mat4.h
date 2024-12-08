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

    Mat4 inverted() const { return Mat4(glm::inverse(data)); };

    Vec3 getLocalForward() { return Vec3(data * glm::vec4(0.0f, 0.0f, -1.0f, 0.0f)); };
    // TODO: figure out how to do this by just getting the columns
    // Vec3 getLocalForward2() { return Vec3(data[2]); };
    Vec3 getLocalRight() { return Vec3(data * glm::vec4(1.0f, 0.0f, 0.0f, 0.0f)); };
    Vec3 getLocalUp() { return Vec3(data * glm::vec4(0.0f, 1.0f, 0.0f, 0.0f)); };

    const glm::mat4& getData() const { return this->data; }

private:
    glm::mat4 data;
};

inline Mat4 operator*(const Mat4& a, const Mat4& b)
{
    return Mat4(a.getData() * b.getData());
}

inline Vec3 operator*(const Mat4& mat, const Vec3& vec)
{
    return Vec3(mat.getData() * vec.getData());
}

inline std::ostream& operator<<(std::ostream& str, Mat4 const& v)
{
    str << glm::to_string(v.getData());
    return str;
}