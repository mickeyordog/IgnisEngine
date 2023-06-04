#pragma once

#include <functional>

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
    const char* name;
    FieldType type;
    const void* ptr;
    std::function<void(void)> postUpdateFunction;

    FieldDescription(const char* name, FieldType type, const void* ptr, const std::function<void(void)> postUpdateFunction = NULL) : name(name), type(type), ptr(ptr), postUpdateFunction(postUpdateFunction) { }
};

enum ComponentType 
{
    CAMERA,
    TRANSFORM,
    SPRITE_RENDERER,
    UNKNOWN
};

struct ComponentClassInfo 
{
    enum ComponentType type;
    const char* name;
    std::function<void*(void)> constructor;
};
