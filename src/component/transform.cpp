#include "transform.h"

#include <iostream>

Transform::Transform(float x, float y, float z) : x(x), y(y), z(z)
{
    for (const auto& field : this->fields)
    {
        if (field.type == Type::Float)
        {
            std::cout << field.name << ": " << *(float*)field.value << std::endl;
        }
    }
}

Transform::~Transform()
{
    
}
