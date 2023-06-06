#include "serializationHelper.h"

std::vector<ComponentClassInfo> SerializationHelper::componentClassInfos;

Component* SerializationHelper::getNewComponent(enum ComponentType componentType)
{
    for (auto& classInfo : componentClassInfos)
    {
        if (classInfo.type == componentType)
            return classInfo.constructor();
    }
    return nullptr;
}

std::unique_ptr<std::vector<const char*>> SerializationHelper::getComponentTypeNames()
{
    auto componentTypeNames = std::make_unique<std::vector<const char*>>();

    for (auto& classInfo : componentClassInfos)
        componentTypeNames->push_back(classInfo.name);

    return componentTypeNames;
}

const char* SerializationHelper::componentTypeToString(enum ComponentType type)
{
    for (auto& classInfo : componentClassInfos)
    {
        if (classInfo.type == type)
            return classInfo.name;
    }
    return nullptr;
}

enum ComponentType SerializationHelper::stringToComponentType(const char* name)
{
    for (auto& classInfo : componentClassInfos)
    {
        if (strcmp(classInfo.name, name) == 0)
            return classInfo.type;
    }
    return ComponentType::UNKNOWN;
}