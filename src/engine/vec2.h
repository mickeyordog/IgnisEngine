#pragma once

#include <stdio.h>
#include <sstream>
#include <vector>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>
#include "serialization.h"
#include "quat.h"

struct Vec2 {
public:
    Vec2() : data(0.0, 0.0) { };
    Vec2(float x) : data(x, x) { };
    Vec2(float x, float y) : data(x, y) { };
    Vec2(glm::vec2 data) : data(data) { };

    float& x() { return data.x; }
    const float& x() const { return data.x; }
    float& y() { return data.y; }
    const float& y() const { return data.y; }

    void normalize() { this->data = glm::normalize(data); };

    const glm::vec2& getData() const { return this->data; }

    Vec2& operator+=(const Vec2& other) { data.x += other.x(); data.y += other.y(); return *this; };
    Vec2& operator*=(float x) { data.x *= x; data.y *= x; return *this; }
    bool operator!=(const Vec2& other) const { return getData() != other.getData(); }

    static const Vec2 RIGHT;
    static const Vec2 UP;

private:
    glm::vec2 data;
};


inline Vec2 operator-(const Vec2& v1, const Vec2& v2) {
    return Vec2(v1.getData() - v2.getData());
}

inline std::ostream& operator<<(std::ostream& str, Vec2 const& v)
{
    str << glm::to_string(v.getData());
    return str;
}