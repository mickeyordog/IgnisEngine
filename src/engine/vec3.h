#pragma once

#include <stdio.h>
#include <vector>
#include <glm/gtc/matrix_transform.hpp>
#include "serialization.h"

class Vec3 {
public:
    Vec3(float x, float y, float z) : data(x, y, z) { };
    ~Vec3();

    const std::vector<FieldDescription>& getFields() { return fields; }

    const glm::vec3 getData() const { return this->data; }
private:
    glm::vec3 data;

    std::vector<FieldDescription> fields = {
        // FieldDescription{GET_NAME(x), FieldType::Float, &x},
        // FieldDescription{GET_NAME(y), FieldType::Float, &y},
        // FieldDescription{GET_NAME(z), FieldType::Float, &z}
    };
};