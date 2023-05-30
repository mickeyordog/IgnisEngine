#pragma once

#include <stdio.h>
#include <vector>
#include "serialization.h"

// TODO: use GLM for backend, for this and matrices
class Vec3 {
public:
    Vec3(float x, float y, float z) : x(x), y(y), z(z) { };
    ~Vec3();

    float x, y, z;

    const std::vector<FieldDescription>& getFields() { return fields; }

private:
    std::vector<FieldDescription> fields = {
        FieldDescription{GET_NAME(x), FieldType::Float, &x},
        FieldDescription{GET_NAME(y), FieldType::Float, &y},
        FieldDescription{GET_NAME(z), FieldType::Float, &z}
    };
};