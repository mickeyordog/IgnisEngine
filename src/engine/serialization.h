#pragma once

#define GET_NAME(variable) (#variable)

enum FieldType
{
    Float,
    Int,
    Subclass,
    EndSubclass,
    ComponentType
};

struct FieldDescription
{
    const char *name;
    FieldType type;
    const void *ptr;
};

struct ClassDescription {

};