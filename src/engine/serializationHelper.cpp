#include "serializationHelper.h"

std::unique_ptr<std::vector<const char*>> SerializationHelper::getComponentTypeNames()
{
    auto componentTypeNames = std::make_unique<std::vector<const char*>>();

    for (auto& kv : componentTypeToConstructor)
        componentTypeNames->push_back(componentTypeToString(kv.first));

    return componentTypeNames;
}

const char* SerializationHelper::componentTypeToString(enum ComponentType type)
{
    switch (type)
    {
    case ComponentType::CAMERA:
        return "Camera";
    case ComponentType::TRANSFORM:
        return "Transform";
    case ComponentType::SPRITE_RENDERER:
        return "Sprite Renderer";
    default:
        return "Unknown";
    }
}

enum ComponentType SerializationHelper::stringToComponentType(const char* name)
{
    if (strcmp(name, "Camera")) {
        return ComponentType::CAMERA;
    } else if (strcmp(name, "Transform")) {
        return ComponentType::TRANSFORM;
    } else if (strcmp(name, "Sprite Renderer")) {
        return ComponentType::SPRITE_RENDERER;
    } else {
        return ComponentType::UNKNOWN;
    }
}