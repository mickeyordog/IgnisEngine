#pragma once

#include <vector>

#define GET_NAME(variable) (#variable)

enum Type
{
    Float,
    Int
};

struct FieldDescription
{
    const char* name;
    Type type;
    const void* ptr;
};

class Component {
public:
    Component();
    ~Component();
    void start();
    void update(float dt);

    const std::vector<FieldDescription>& getFields() { return fields; };

private:
    std::vector<FieldDescription> fields = { };
};


