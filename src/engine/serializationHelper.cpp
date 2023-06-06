#include "serializationHelper.h"

std::vector<ComponentClassInfo> SerializationHelper::componentClassInfos;
std::vector<const char*> SerializationHelper::componentTypeNames;

void SerializationHelper::registerComponentClass(ComponentClassInfo componentClassInfo)
{
    componentClassInfos.push_back(componentClassInfo);
    componentTypeNames.push_back(componentClassInfo.name);
}


Component* SerializationHelper::getNewComponent(enum ComponentType componentType)
{
    for (auto& classInfo : componentClassInfos)
    {
        if (classInfo.type == componentType)
            return classInfo.constructor();
    }
    return nullptr;
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

