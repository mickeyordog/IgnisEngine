#pragma once

#define GET_NAME(variable) (#variable)

enum FieldType
{
    Float,
    Int,
    Subclass,
    EndSubclass,
    ComponentType,
    vec3
};

struct FieldDescription
{
    const char *name;
    FieldType type;
    const void *ptr;
    const void* objectPtr;

    FieldDescription(const char *name, FieldType type, const void *ptr, const void* objectPtr = nullptr) : name(name), type(type), ptr(ptr), objectPtr(objectPtr) {}
};

struct ClassDescription {
    // const char* name;
    // void* constructor;
};