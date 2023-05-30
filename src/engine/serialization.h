#pragma once

#define GET_NAME(variable) (#variable)

enum Type
{
    Float,
    Int
};

struct FieldDescription
{
    const char *name;
    Type type;
    const void *ptr;
};