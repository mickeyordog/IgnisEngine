#pragma once

#include <functional>

#define GET_NAME(variable) (#variable)

enum class FieldType
{
    FLOAT_FIELD,
    INT_FIELD,
    LLONG_FIELD,
    BOOL_FIELD,
    STRING_FIELD,
    VEC3_FIELD,
    ASSET_POINTER_FIELD,
    COMPONENT_POINTER_FIELD,
};

struct FieldDescription
{
    const char* name;
    FieldType type;
    void* ptr;
    std::function<void(void)> postUpdateFunction;
    const char* validFileExtension;

    FieldDescription(const char* name, FieldType type, void* ptr, const std::function<void(void)> postUpdateFunction = []() { }, const char* validFileExtension = "")
        : name(name), type(type), ptr(ptr), postUpdateFunction(postUpdateFunction), validFileExtension(validFileExtension) { }
};

enum class ComponentType
{
    CAMERA = 0,
    TRANSFORM = 1,
    SPRITE_RENDERER = 2,
    ANIMATOR = 3,
    UNKNOWN = 4
};

class Component;
struct ComponentClassInfo
{
    enum ComponentType type;
    const char* name;
    std::function<Component* (void)> constructor;
};
