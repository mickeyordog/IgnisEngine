#pragma once

#include "component.h"

class Transform {
public:
    Transform(float x, float y, float z);
    ~Transform();

    void start();
    void update(float dt);
    void serialize();
    void deserialize();
    float x, y, z;

    std::vector<FieldInfoTest> fields = {
        FieldInfoTest{GET_NAME(x), Type::Float, &x},
        FieldInfoTest{GET_NAME(y), Type::Float, &y},
        FieldInfoTest{GET_NAME(z), Type::Float, &z}};
};