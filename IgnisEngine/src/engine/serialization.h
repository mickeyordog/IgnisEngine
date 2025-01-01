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

// Could optimize this by just storing an offset instead of a ptr, then could be static per class
struct FieldDescription
{
    const char* name;
    FieldType type;
    void* ptr;
    std::function<void(void)> postUpdateFunction; // When I make the fielddescriptions static, might need to pass in component to function

    const char* validFileExtension; // TODO: This should be replaced with an AssetType
    std::function<bool(void)> showConditionFunction;

    FieldDescription(const char* name, FieldType type, void* ptr, const std::function<void(void)> postUpdateFunction = []() { }, const char* validFileExtension = "", const std::function<bool(void)> showConditionFunction = []() { return true; })
        : name(name), type(type), ptr(ptr), postUpdateFunction(postUpdateFunction), validFileExtension(validFileExtension), showConditionFunction(showConditionFunction) { }
};

// I could actually maybe just generate a guid for each component type and use that for serialization
enum class ComponentType
{
    CAMERA,
    TRANSFORM,
    SPRITE_RENDERER,
    ANIMATOR,
    MESH_RENDERER,
    FIRST_PERSON_CONTROLLER,
    LIGHT,
    RIGID_BODY,
    UNKNOWN
};

class Component;
struct ComponentClassInfo
{
    enum ComponentType type;
    const char* name;
    std::function<Component* (void)> constructor;
};
