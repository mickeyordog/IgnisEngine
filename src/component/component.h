#pragma once

#include <vector>

enum Type
{
    Float,
    Int
};

struct FieldInfoTest
{
    const char* name;
    Type type;
    const void* value;
};

#define GET_NAME(variable) (#variable)

class Component {
public:
    Component();
    ~Component();
    virtual void start() = 0;
    virtual void update(float dt) = 0;
    virtual void serialize() = 0;
    virtual void deserialize() = 0;
};


