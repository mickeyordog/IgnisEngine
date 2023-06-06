#pragma once

#include <functional>

#define GET_NAME(variable) (#variable)

enum FieldType
{
    FLOAT,
    INT,
    BOOL,
    VEC3,
    POINTER
};

struct FieldDescription
{
    const char* name;
    FieldType type;
    const void* ptr;
    std::function<void(void)> postUpdateFunction;

    FieldDescription(const char* name, FieldType type, const void* ptr, const std::function<void(void)> postUpdateFunction = []() { })
        : name(name), type(type), ptr(ptr), postUpdateFunction(postUpdateFunction) { }
};

enum class ComponentType
{
    CAMERA,
    TRANSFORM,
    SPRITE_RENDERER,
    ANIMATOR,
    UNKNOWN
};

class Component;
struct ComponentClassInfo
{
    enum ComponentType type;
    const char* name;
    std::function<Component* (void)> constructor;
};
